#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tools/constants.h"
#include "semaphore.h"

#ifndef OS_LOG_SYSTEM_LOADED
#define OS_LOG_SYSTEM_LOADED
#include "../terminal/log.h"
#endif 

#if OS_DEBUG || OS_SEM_DEBUG
#define LOG_SEM(fmt) printf(fmt)
#define LOG_SEM_A(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define LOG_SEM(fmt)
#define LOG_SEM_A(fmt, ...)
#endif 

/**
 * Inicializa um semáforo com nome e valor inicial S.
 * Também configura mutex e lista de espera.
 */
void IniciaSemaforo(semaphore_t *sem, const char *name, const int S) {
    sem->name    = strdup(name);
    sem->S       = S;
    sem->waiters = IniciaLista();      // Lista de processos esperando
    sem_init(&sem->mutex, 0, 1);    
}

/**
 * Registra um novo semáforo na tabela, se ainda não existir.
 * Usa realloc para expandir a tabela dinamicamente.
 */
void semaphore_register(semaphore_table_t *sem_table, const char *name) {
    semaphore_t *sem;
    semaphore_t *n_table;
    register int t;
    
    // Verifica se o semáforo já foi registrado
    for (t = 0; t < sem_table->len; t++)
    {
      if (0 == strcmp(sem_table->table[t].name, name))
      {
        LOG_SEM_A("Semaphore %s ja registrado.\n", name);
        return;
      }
    }

        // Realoca a tabela para adicionar novo semáforo
    n_table = (semaphore_t *)
            realloc(sem_table->table, sizeof(semaphore_t) * (sem_table->len + 1));


    if (!n_table) {
        printf("Sem memoria... \n");
        exit(EXIT_FAILURE);
    }

    sem_table->table = n_table;
    sem_table->len   = sem_table->len + 1;
    sem              = &sem_table->table[sem_table->len-1];

    IniciaSemaforo(sem, name, 1); // Inicializa novo semáforo com valor 1

    LOG_SEM_A("Semaphore %s ja registrado.\n", sem->name);
}


semaphore_t *BuscaSemaforo(semaphore_table_t *sem_table, const char *name) {
    semaphore_t *sem;
    for (sem = sem_table->table; sem < &sem_table->table[sem_table->len]; sem++)
      if (0 == strcmp(sem->name, name))
        return sem;
    LOG_SEM_A("%s não encontrado.\n", name);
    return NULL;
}


void semaphore_P(semaphore_t *sem, process_t *proc, void (*sleep)(void)) {
    int blocked = 0;

    sem_wait(&sem->mutex);
    sem->S--;
    if (sem->S < 0) {
        AdicionaLista(sem->waiters, proc);
        sleep();

        blocked = 1;
    }

    res_acq_log(proc->name, sem->name, 1, blocked);
    sem_post(&res_acq_mutex);

    sem_post(&sem->mutex);
}

void semaphore_V(semaphore_t *sem, process_t *process, void (*wakeup)(process_t *)) {
    sem_wait(&sem->mutex);
    sem->S++;
    if (sem->S <= 0) {
        process_t* proc = list_remove_head(sem->waiters)->content;
        wakeup(proc);
    }

    res_acq_log(process->name, sem->name, 0, 0);
    sem_post(&res_acq_mutex);

    sem_post(&sem->mutex);
}

void semaphore_table_init(semaphore_table_t *sem_table) {
  semaphore_t *n_table;

  n_table = (semaphore_t *)malloc(0);

  if (!n_table) {
      printf("Sem memoria... \n");
      exit(EXIT_FAILURE);
  }

  sem_table->table = n_table;
  sem_table->len   = 0;
}
