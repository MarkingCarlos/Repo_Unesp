#include <stdio.h>

#include "../tools/constants.h"
#include "../tools/math.h"
#include "kernel.h"

#if OS_DEBUG || OS_KERNEL_DEBUG
#define LOG_KERNEL(fmt) printf(fmt)
#define LOG_KERNEL_A(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define LOG_KERNEL(fmt)
#define LOG_KERNEL_A(fmt, ...)
#endif 

#if OS_DEBUG || OS_KERNEL_DEBUG || OS_KERNEL_EVAL_DEBUG
#define LOG_KERNEL_EVAL(fmt) printf(fmt)
#define LOG_KERNEL_EVAL_A(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define LOG_KERNEL_EVAL(fmt)
#define LOG_KERNEL_EVAL_A(fmt, ...)
#endif 


static void sleep();


static void wakeup(process_t* proc);

void IniciaKernel() {
    kernel = (kernel_t *)malloc(sizeof(kernel_t));

    if (!kernel) {
        printf("Memoria insuficiente\n");
        exit(0);
    }

    kernel->proc_table = IniciaLista();
    kernel->next_proc_id = 1; 
    LOG_KERNEL("Process table iniciado.\n");

    segment_table_init(&kernel->seg_table);
    LOG_KERNEL("Segment table iniciado.\n");

    InicializadorScheduler(&kernel->scheduler);
    LOG_KERNEL("Scheduler iniciado.\n");

    disk_InicializadorScheduler(&kernel->disk_scheduler);
    LOG_KERNEL("Disk Scheduler iniciado.\n");

    semaphore_table_init(&kernel->sem_table);
    LOG_KERNEL("Semaphore table iniciado.\n");

    file_table_init(&kernel->file_table);
    LOG_KERNEL("File table initialized.\n");
    LOG_KERNEL("Kernel initialized.\n");
}

void sysCall(kernel_function_t func, void *arg) {
    switch (func) {
        case PROCESS_INTERRUPT: {
            ProcessoScheduler(&kernel->scheduler, (scheduler_flag_t)arg);
            break;
        }
        case PROCESS_CREATE: {
            process_create((char *)arg);
            break;
        }
        case PROCESS_FINISH: {
            process_finish((process_t *)arg);
            break;
        }
        case MEM_LOAD_REQ: {

            mem_req_load((memory_request_t *)arg, &kernel->seg_table);

       
            interruptControl(MEM_LOAD_FINISH, (memory_request_t *)arg);
            break;
        }
        case SEMAPHORE_P: {
            semaphore_P((semaphore_t *)arg, kernel->scheduler.scheduled_proc, sleep);
            break;
        }
        case SEMAPHORE_V: {
            semaphore_V((semaphore_t *)arg, kernel->scheduler.scheduled_proc, wakeup);
            break;
        }
        case DISK_READ_REQUEST: {
            const int track = (int)arg;
            process_t* curr_proc = kernel->scheduler.scheduled_proc;

            
            ProcessoScheduler(&kernel->scheduler, IO_REQUESTED);

            
            disk_request(curr_proc, &kernel->disk_scheduler, track, 1);

            fs_op_request_t fs_req;
            fs_req.proc = curr_proc;
            fs_req.track = track;
            fs_req.read = 1;


            sysCall(FS_REQUEST, (void *)(&fs_req));
            break;
        }
        case DISK_WRITE_REQUEST: {
            const int track = (int)arg;
            process_t* curr_proc = kernel->scheduler.scheduled_proc;

            
            ProcessoScheduler(&kernel->scheduler, IO_REQUESTED);

            
            disk_request(curr_proc, &kernel->disk_scheduler, track, 0);

            fs_op_request_t fs_req;
            fs_req.proc = curr_proc;
            fs_req.track = track;
            fs_req.read = 0;

            sysCall(FS_REQUEST, (void *)(&fs_req));
            break;
        }
        case PRINT_REQUEST: {
            process_t* curr_proc = kernel->scheduler.scheduled_proc;

            
            ProcessoScheduler(&kernel->scheduler, IO_REQUESTED);

            print_request(curr_proc, (int)arg);
            break;
        }
        case FS_REQUEST: {
            fs_op_request_t* fs_req = (fs_op_request_t *)arg;

            if (fs_req->read)
                fs_read_request(&kernel->file_table, fs_req->proc, DISK_BLOCK(fs_req->track));
            else fs_write_request(&kernel->file_table, fs_req->proc, DISK_BLOCK(fs_req->track));
            break;
        }
    }
}


void interruptControl(kernel_function_t func, void *arg) {
    switch (func) {
        case MEM_LOAD_FINISH: {
            memory_request_t* req = (memory_request_t *)arg;
            process_t* proc = req->proc;

            AdicionaLista(kernel->proc_table, proc);

            proc->state = READY;
            if (proc->priority == 1)
                AdicionaLista(kernel->scheduler.high_queue->queue, proc);
            else AdicionaLista(kernel->scheduler.low_queue->queue, proc);
            break;
        }
        
        case DISK_FINISH: {
            DesbloquearProcesso(&kernel->scheduler, (process_t *)arg, LOW_QUEUE);
            break;
        }
        
        case PRINT_FINISH: {
            DesbloquearProcesso(&kernel->scheduler, (process_t *)arg, LOW_QUEUE);
            break;
        }
    }
}

void eval(process_t* proc, instr_t* instr) {
    switch (instr->op) {
    case EXEC: {
        LOG_KERNEL_EVAL_A("Process %s vai executar %d u.t.\n", proc->name, instr->value);

        proc->remaining = proc->remaining - instr->value;

        LOG_KERNEL_EVAL_A("Process %s terminou a execucao.\n", proc->name);
        break;
    }
    case SEM_P: {
        LOG_KERNEL_EVAL_A("Process %s solicitou um semaforo%s.\n", proc->name, instr->sem);

        sysCall(SEMAPHORE_P, BuscaSemaforo(&kernel->sem_table, instr->sem));

        if (proc->state != BLOCKED)
            proc->remaining = MAX(0, proc->remaining - 200);
        break;
    }
    case SEM_V: {
        LOG_KERNEL_EVAL_A("Process %s iniciou um semaforo%s.\n", proc->name, instr->sem);

        sysCall(SEMAPHORE_V, BuscaSemaforo(&kernel->sem_table, instr->sem));
        proc->remaining = MAX(0, proc->remaining - 200);
        break;
    }
    case READ: {
        proc->io_read_count++;
        LOG_KERNEL_EVAL_A("Process %s requisitou uma op de leitura %d.\n", proc->name, instr->value);

        sysCall(DISK_READ_REQUEST, instr->value);
        break;
    }
    case WRITE: {
        proc->io_write_count++
        LOG_KERNEL_EVAL_A("Process %s requisitou uma op de escrita %d.\n", proc->name, instr->value);

        sysCall(DISK_WRITE_REQUEST, instr->value);
        break;
    }
    case PRINT: {
        LOG_KERNEL_EVAL_A("Process %s requisitou uma op de print  %d u.t.\n", proc->name, instr->value);

        sysCall(PRINT_REQUEST, instr->value);
        break;
    }
    }
}

static void sleep() {

    sysCall(PROCESS_INTERRUPT, (void *) SEMAPHORE_BLOCKED);
}


static void wakeup(process_t* proc) {
    DesbloquearProcesso(&kernel->scheduler, proc, HIGH_QUEUE);
}
