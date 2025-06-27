#include <time.h>

#ifndef OS_CPU_LOADED
#define OS_CPU_LOADED
#include "cpu/cpu.h"
#endif 

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual

    // Inicializa os arquivos de log para diferentes componentes do sistema
    process_log_init();   // Log de processos
    disk_log_init();      // Log de operações de disco
    io_log_init();        // Log de operações de entrada/saída
    res_acq_log_init();   // Log de aquisição de recursos (semáforos, etc.)

    // Inicializa os módulos principais do sistema
    IniciaKernel();       // Inicializa o kernel (núcleo do SO)
    IniciaDisco();        // Inicializa o simulador de disco
    cpu_init();         

    // Inicia o terminal para interação com o usuário
    begin_terminal();
    return 0;
}

