#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE semaforo = CreateSemaphore(
        NULL,  // Atributos de segurança
        1,     // Valor inicial
        1,     // Valor máximo
        NULL   // Nome (NULL para anônimo)
    );

    // Espera o semáforo
    WaitForSingleObject(semaforo, INFINITE);

    printf("Dentro da secao critica\n");

    // Libera o semáforo
    ReleaseSemaphore(semaforo, 1, NULL);

    CloseHandle(semaforo);
    return 0;
}
