#ifndef OS_PROJECT_CPU_H
#define OS_PROJECT_CPU_H

#ifndef OS_CPU_KERNEL_LOADED
#define OS_CPU_KERNEL_LOADED
#include "../kernel/kernel.h"
#endif 

void cpu_init();


_Noreturn void cpu();

#endif 