#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
/**
 * Processa uma instrução de semáforo do tipo P(x) ou V(x).
 * Valida se o semáforo está registrado e define o tipo da operação.
 */
void instr_semaphore_parse(instr_t* instr, const char* line, semaphore_table_t* sem_table) {
    char* sem_name = strdup(line);
    semaphore_t* sem;

    sem_name = sem_name + 2; 
    sem_name[strlen(sem_name) - 2] = '\0';

    sem = BuscaSemaforo(sem_table, sem_name);

    if (!sem) {
        printf("A semaphore with the name %s has not been registered.\n", sem_name);
        exit(0);
    }

    instr->op = line[0] == 'P' ? SEM_P : SEM_V;
    instr->sem = sem_name;
}

/**
 * Interpreta a linha textual e determina a instrução.
 * Pode ser uma operação comum (exec, read, etc.) ou de semáforo.
 */
void instr_parse(instr_t* instr, const char* line, semaphore_table_t* sem_table) {
    if (line[0] == 'P' || line[0] == 'V')
        instr_semaphore_parse(instr, line, sem_table);
        
    else {
        char* dupline = strdup(line);
        char* left_op = strtok(dupline, " ");
        int right_op = atoi(strtok(NULL, " "));

        if (strcmp(left_op, "exec") == 0){
            instr->op = EXEC;
        }
            
        else if (strcmp(left_op, "read") == 0)
            instr->op = READ;
        else if (strcmp(left_op, "write") == 0)
            instr->op = WRITE;
        else if (strcmp(left_op, "print") == 0)
            instr->op = PRINT;

        instr->value = right_op;
        instr->sem = NULL;
    }
}
