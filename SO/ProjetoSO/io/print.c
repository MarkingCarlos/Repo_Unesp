#include "print.h"

#ifndef OS_SCHED_KERNEL
#define OS_SCHED_KERNEL
#include "../kernel/kernel.h"
#endif 

void print_request(process_t* process, int duration) {
    io_print_log(process->name, duration);
    sem_post(&io_mutex);

    process->remaining -= duration;
    interruptControl(PRINT_FINISH, process);
}
