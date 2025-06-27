#ifndef OS_PROJECT_KERNEL_H
#define OS_PROJECT_KERNEL_H

#include "../memory/memory.h"
#include "../sched/sched.h"
#include "../process/instruction.h"
#include "../disk/disk.h"
#include "../io/print.h"
#include "../file/file.h"

typedef enum KernelFunction {

    PROCESS_INTERRUPT = 1,
    PROCESS_CREATE = 2,
    PROCESS_FINISH = 3,

    DISK_READ_REQUEST = 4,
    DISK_WRITE_REQUEST = 16,
    DISK_FINISH = 5,


    MEM_LOAD_REQ = 6,
    MEM_LOAD_FINISH = 7,

    FS_REQUEST = 8,
    FS_FINISH = 9,

    SEMAPHORE_P = 10,
    SEMAPHORE_V = 11,

    PRINT_REQUEST = 14,
    PRINT_FINISH = 15
} kernel_function_t;

typedef struct Kernel {

    list_t* proc_table;
    int next_proc_id;

    segment_table_t seg_table;

    scheduler_t scheduler;

    disk_scheduler_t disk_scheduler;

    semaphore_table_t sem_table;

    file_table_t file_table;

    int pc; 
} kernel_t;


extern kernel_t* kernel;


void IniciaKernel();

void interruptControl(kernel_function_t func, void *arg);

void sysCall(kernel_function_t func, void *arg);


void eval(process_t* proc, instr_t* instr);

#endif 
