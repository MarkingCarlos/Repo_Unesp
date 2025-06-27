#ifndef SO_PROJECT_INSTRUCTION_H
#define SO_PROJECT_INSTRUCTION_H

#include "../semaphore/semaphore.h"

// Enumeração de instruções que um processo pode executar
typedef enum Opcode {
    EXEC,    // Executar por um tempo
    READ,    // Leitura de disco
    WRITE,   // Escrita em disco
    PRINT,   // Imprimir no terminal
    SEM_P,   // Operação P (down) em semáforo
    SEM_V    // Operação V (up) em semáforo
} opcode_t;

// Representação de uma instrução individual
typedef struct Instruction {
    opcode_t op;   // Operação
    int value;     // Valor associado (tempo ou trilha, por exemplo)
    char* sem;     // Nome do semáforo (se aplicável)
} instr_t;

/**
 * Interpreta a linha de instrução textual e preenche a estrutura instr_t.
 * Se for uma operação com semáforo, também usa a tabela de semáforos.
 *
 * @param instr estrutura a ser preenchida
 * @param line linha de instrução lida do arquivo
 * @param sem_table tabela de semáforos
 */
void instr_parse(instr_t* instr, const char* line, semaphore_table_t* sem_table);

#endif // SO_PROJECT_INSTRUCTION_H
