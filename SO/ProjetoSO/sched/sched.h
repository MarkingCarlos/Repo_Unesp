#ifndef OS_PROJECT_SCHED_H
#define OS_PROJECT_SCHED_H

#include "../tools/list.h"
#include "../process/process.h"

// Flags que sinalizam eventos que afetam o escalonamento
typedef enum SchedulerFlags {
    NONE = 0x0,               // Nenhuma flag
    IO_REQUESTED = 0x1,       // Processo solicitou I/O
    QUANTUM_COMPLETED = 0x2,  // Processo esgotou seu quantum
    SEMAPHORE_BLOCKED = 0x4   // Processo bloqueado por semáforo
} scheduler_flag_t;

// Identifica a fila de destino ao desbloquear um processo
typedef enum SchedulerQueueFlag {
    LOW_QUEUE = 0,   // Fila de baixa prioridade
    HIGH_QUEUE = 1   // Fila de alta prioridade
} scheduler_queue_flag_t;

// Representa uma fila de processos com um quantum associado
typedef struct SchedulerQueue {
    list_t* queue;  // Lista de processos
    int quantum;    // Tempo de CPU permitido para processos da fila
} scheduler_queue_t;

// Estrutura principal do escalonador
typedef struct Scheduler {
    process_t* scheduled_proc;         // Processo atualmente escalonado
    scheduler_queue_t* high_queue;     // Fila de alta prioridade
    scheduler_queue_t* low_queue;      // Fila de baixa prioridade
    scheduler_queue_t* blocked_queue;  // Fila de processos bloqueados
} scheduler_t;

// Inicializa o escalonador e suas filas
void InicializadorScheduler(scheduler_t* scheduler);

// Escolhe o próximo processo a ser executado, com base em flags
void ProcessoScheduler(scheduler_t* scheduler, scheduler_flag_t flags);

// Move um processo da fila de bloqueados para a fila apropriada
void DesbloquearProcesso(scheduler_t* scheduler, process_t* proc, scheduler_queue_flag_t queue_flag);

#endif 
