#ifndef OS_PROJECT_SEMAPHORE_H
#define OS_PROJECT_SEMAPHORE_H

#include <semaphore.h>

#include "../tools/list.h"
#include "../process/process.h"

typedef struct Semaphore {

    char   *name;

    int    S;

    list_t *waiters;

    sem_t  mutex;
} semaphore_t;

typedef struct SemaphoreTable {

    semaphore_t *table;


    int          len;
} semaphore_table_t;


void IniciaSemaforo(semaphore_t *sem, const char *name, int S);


void semaphore_register(semaphore_table_t *sem_table, const char *name);


semaphore_t *BuscaSemaforo(semaphore_table_t *sem_table, const char *name);


void semaphore_P(semaphore_t *sem, process_t *proc, void (*sleep)(void));


void semaphore_V(semaphore_t *sem, process_t *process, void (*wakeup)(process_t *));


void semaphore_table_init(semaphore_table_t *sem_table);

#endif 
