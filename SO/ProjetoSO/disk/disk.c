#include <pthread.h>
#include <time.h>
#include <math.h>
#include <limits.h> // Para INT_MAX

#include "disk.h"

#ifndef OS_SCHED_KERNEL
#define OS_SCHED_KERNEL
#include "../kernel/kernel.h"
#include "../terminal/terminal.h"
#endif

#define MILLISECONDS_100 (50000000L) // 50ms em nanossegundos

// Protótipos
static disk_request_t* find_next_sstf_request(disk_scheduler_t* scheduler, int current_track);
static void rw_disk_op_resolve(int track);

// Inicialização do disco
void IniciaDisco() {
    pthread_t disk_id;
    pthread_attr_t disk_attr;

    pthread_attr_init(&disk_attr);
    pthread_attr_setscope(&disk_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&disk_id, NULL, (void*)disk, NULL);
}

// Loop principal do disco (SSTF)
_Noreturn void disk() {
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    while (1) {
        clock_gettime(CLOCK_REALTIME, &end);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);

        if (elapsed >= MILLISECONDS_100) {
            start = end;

            // Seleciona a requisição mais próxima (SSTF)
            disk_request_t* next_request = find_next_sstf_request(
                &kernel->disk_scheduler, 
                kernel->disk_scheduler.curr_track
            );

            if (next_request) {
                // Move a cabeça para a trilha da requisição
                kernel->disk_scheduler.curr_track = next_request->track;
                
                // Processa requisições na trilha atual
                rw_disk_op_resolve(kernel->disk_scheduler.curr_track);
            }

            // Atualiza logs
            disk_general_log->forward_dir = kernel->disk_scheduler.forward_dir;
            disk_general_log->curr_track = kernel->disk_scheduler.curr_track;
            disk_general_log->angular_v = (int)(DISk_BASE_ANGULAR_V + 100 * sin(kernel->disk_scheduler.curr_track));
            disk_general_log->pending_requests_size = kernel->disk_scheduler.pending_requests->size;
        }
    }
}

// Inicializa o escalonador do disco
void disk_InicializadorScheduler(disk_scheduler_t* disk_scheduler) {
    disk_scheduler->pending_requests = IniciaLista();
    disk_scheduler->forward_dir = 1;
    disk_scheduler->curr_track = 0;
}

// Adiciona uma requisição ao disco (com tempo mínimo de 4000)
void disk_request(process_t* process, disk_scheduler_t* disk_scheduler,
                 int track, int read) {
    // Calcula o tempo de turnaround (mínimo 4000)
    int distance = abs(disk_scheduler->curr_track - track);
    int time = DISK_OPERATION_TIME + (distance * DISK_TRACK_MOVE_TIME);

    // Garante tempo mínimo de 4000
    if (time < DISK_OPERATION_TIME) {
        time = DISK_OPERATION_TIME;
    }

    // Cria e enfileira a requisição
    disk_request_t* disk_req = create_disk_request();
    disk_req->process = process;
    disk_req->track = track;
    disk_req->read = read;
    disk_req->turnaround = time;

    // Logs e sincronização
    io_disk_log(process->name, read);
    sem_post(&io_mutex);
    AdicionaLista(disk_scheduler->pending_requests, disk_req);
    process->remaining -= time;
}

// Cria uma nova requisição de disco
disk_request_t* create_disk_request() {
    disk_request_t* disk_req = malloc(sizeof(disk_request_t));
    if (!disk_req) {
        printf("Erro: Memória insuficiente\n");
        exit(EXIT_FAILURE);
    }
    return disk_req;
}

// Encontra a requisição mais próxima (SSTF)
static disk_request_t* find_next_sstf_request(disk_scheduler_t* scheduler, int current_track) {
    if (list_empty(scheduler->pending_requests)) return NULL;

    disk_request_t* closest_request = NULL;
    int min_distance = INT_MAX;

    FOREACH(scheduler->pending_requests, disk_request_t*) {
        int distance = abs(current_track - it->track);
        if (distance < min_distance) {
            min_distance = distance;
            closest_request = it;
        }
    }
    return closest_request;
}

// Processa requisições na trilha especificada
static void rw_disk_op_resolve(int track) {
    list_node_t* curr_node = kernel->disk_scheduler.pending_requests->head;
    while (curr_node) {
        list_node_t* next_node = curr_node->next;
        disk_request_t* disk_req = (disk_request_t*)curr_node->content;

        if (disk_req->track == track) {
            // Registra operações
            disk_log(disk_req->process->name, disk_req->process->id, track, disk_req->read, disk_req->turnaround);
            io_fs_log(disk_req->process->name, INODE_NUMBER(DISK_BLOCK(track)),
                     disk_req->read ? IO_LOG_FS_READ : IO_LOG_FS_WRITE);

            // Sincronização e finalização
            sem_post(&disk_mutex);
            sem_post(&io_mutex);
            interruptControl(DISK_FINISH, disk_req->process);

            // Remove a requisição atendida
            RemoveNode(kernel->disk_scheduler.pending_requests, curr_node);
        }
        curr_node = next_node;
    }
}
