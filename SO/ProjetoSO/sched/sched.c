#include <stdio.h>
#include <stdlib.h>

#include "../tools/math.h"
#include "sched.h"

// Definem os quantums para as filas de alta e baixa prioridade
#define SCHED_HIGH_QUEUE_QUANTUM (5000)
#define SCHED_LOW_QUEUE_QUANTUM (2500)

// Cria uma fila de escalonamento com o quantum especificado
scheduler_queue_t* create_queue(int quantum);

// Inicializa todas as filas do escalonador
void InicializadorScheduler(scheduler_t* scheduler) {
    scheduler->scheduled_proc = NULL;
    scheduler->high_queue = create_queue(SCHED_HIGH_QUEUE_QUANTUM);
    scheduler->low_queue = create_queue(SCHED_LOW_QUEUE_QUANTUM);
    scheduler->blocked_queue = create_queue(-1);
}

void ProcessoScheduler(scheduler_t* scheduler, scheduler_flag_t flags) {
    process_t* curr_scheduled = scheduler->scheduled_proc;
    process_t* new_scheduled = NULL;

    // Procurar na high_queue o processo com mais operações de E/S
    process_t* selected_proc = NULL;
    list_node_t* node = scheduler->high_queue->queue->head;

    while (node) {
        process_t* p = (process_t*)node->content;
        int p_io = p->io_read_count + p->io_write_count;
        int selected_io = selected_proc ? selected_proc->io_read_count + selected_proc->io_write_count : -1;

        // Seleciona o processo com maior I/O, desempate por menor ID
        if (!selected_proc ||
            (p_io > selected_io) ||
            (p_io == selected_io && p->id < selected_proc->id)) {
            selected_proc = p;
        }

        node = node->next;
    }

    if (selected_proc) {
        list_remove(scheduler->high_queue->queue, selected_proc);
        new_scheduled = selected_proc;
        new_scheduled->remaining = MAX(1000, MIN(SCHED_HIGH_QUEUE_QUANTUM,
                                                 new_scheduled->remaining + scheduler->high_queue->quantum));
    } else if (!list_empty(scheduler->low_queue->queue)) {
        new_scheduled = (process_t *)list_remove_head(scheduler->low_queue->queue)->content;
        if (new_scheduled)
            new_scheduled->remaining = MAX(0, MIN(SCHED_LOW_QUEUE_QUANTUM,
                                                  new_scheduled->remaining + scheduler->low_queue->quantum));
    }

    // Recoloca o processo atual em fila apropriada ou bloqueia
    if (curr_scheduled) {
        if ((flags & IO_REQUESTED) || (flags & SEMAPHORE_BLOCKED)) {
            AdicionaLista(scheduler->blocked_queue->queue, curr_scheduled);
        } else if (curr_scheduled->remaining > 0) {
            AdicionaLista(scheduler->low_queue->queue, curr_scheduled);
        } else {
            AdicionaLista(scheduler->high_queue->queue, curr_scheduled);
        }
    }

    scheduler->scheduled_proc = new_scheduled;
}



void DesbloquearProcesso(scheduler_t* scheduler, process_t* proc, scheduler_queue_flag_t queue_flag) {
    list_node_t* proc_node;

    if (!(proc_node = BuscaLista(scheduler->blocked_queue->queue, proc, proc_cmp)))
        return;

    RemoveNode(scheduler->blocked_queue->queue, proc_node);

    switch (queue_flag) {
        case HIGH_QUEUE: {
            AdicionaLista(scheduler->high_queue->queue, proc);
            break;
        }
        case LOW_QUEUE: {
            AdicionaLista(scheduler->low_queue->queue, proc);
            break;
        }
        default: {
            printf("Unknown scheduler queue.\n");
            exit(EXIT_FAILURE);
        }
    }
}


scheduler_queue_t* create_queue(int quantum) {
    scheduler_queue_t* queue = (scheduler_queue_t *)
            malloc(sizeof(scheduler_queue_t));

    if (!queue) {
        printf("Sem memoria... \n");
        exit(0);
    }

    queue->queue = IniciaLista();
    queue->quantum = quantum;
    return queue;
}