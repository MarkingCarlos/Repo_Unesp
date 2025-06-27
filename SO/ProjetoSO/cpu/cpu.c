#include <curses.h>
#include <pthread.h>
#include <time.h>

#include "../terminal/terminal.h"
#include "cpu.h"

#define ONE_SECOND_NS (1000000000L) // Constante para representar um segundo em nanossegundos

// Ponteiro global para a estrutura do kernel
kernel_t* kernel;

/**
 * Inicializa a CPU criando uma thread separada
 * que executará a função `cpu()`, simulando o funcionamento da CPU.
 */
void cpu_init() {
    pthread_t cpu_id;             // ID da thread da CPU
    pthread_attr_t cpu_attr;     // Atributos da thread

    pthread_attr_init(&cpu_attr);
    pthread_attr_setscope(&cpu_attr, PTHREAD_SCOPE_SYSTEM);

    pthread_create(&cpu_id, NULL, (void*)cpu, NULL);
}

/**
 * Função executada pela thread da CPU.
 * Fica em loop infinito buscando o próximo processo escalonado
 * e executa instruções a cada 1 segundo simulado.
 */
_Noreturn void cpu() {

    while (!kernel)
        ;  // Aguarda até que o kernel esteja inicializado

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_REALTIME, &start); // Marca o tempo inicial

    int no_process = 0; // Flag para indicar ausência de processo

    while (1) {
        // Se não há processo escalonado
        if (!kernel->scheduler.scheduled_proc) {
            if (!no_process) {
                process_np_log();        // Log de ausência de processo
                sem_post(&log_mutex);    // Libera semáforo de log
                sem_post(&refresh_sem);  // Atualiza tela/terminal
                no_process = 1;
            }

            // Chama o escalonador de processos
            ProcessoScheduler(&kernel->scheduler, NONE);
        }

        else {
            no_process = 0;
            do {
                clock_gettime(CLOCK_REALTIME, &end);
                const int elapsed = (end.tv_sec - start.tv_sec) * ONE_SECOND_NS
                                    + (end.tv_nsec - start.tv_nsec);

                if (elapsed >= ONE_SECOND_NS) {
                    start = end;

                    const int pc
                        = FETCH_INSTR_ADDR(kernel->scheduler.scheduled_proc);
                    const int page_number = PAGE_NUMBER(pc);
                    const int page_offset = PAGE_OFFSET(pc);


                    segment_t* seg = ProcuraSegmento(
                        &kernel->seg_table,
                        kernel->scheduler.scheduled_proc->seg_id);
                    page_t* page = &seg->page_table[page_number];
                    instr_t instr = page->code[page_offset];

                    if (!page->used)
                        page->used = 1;

                    process_log(kernel->scheduler.scheduled_proc->name,
                                kernel->scheduler.scheduled_proc->remaining,
                                pc,
                                seg->id,
                                kernel->scheduler.scheduled_proc->o_files->size);
                    sem_post(&log_mutex);
                    sem_post(&refresh_sem);

                    eval(kernel->scheduler.scheduled_proc, &instr);
                }
            } while (kernel->scheduler.scheduled_proc != NULL
                     && kernel->scheduler.scheduled_proc->remaining > 0
                     && kernel->scheduler.scheduled_proc->pc
                            < kernel->scheduler.scheduled_proc->code_len);

            if (kernel->scheduler.scheduled_proc == NULL)
                continue;

            if (kernel->scheduler.scheduled_proc->pc
                >= kernel->scheduler.scheduled_proc->code_len)
                sysCall(PROCESS_FINISH, kernel->scheduler.scheduled_proc);
            else

                sysCall(PROCESS_INTERRUPT, (void*)QUANTUM_COMPLETED);
        }
    }
}
