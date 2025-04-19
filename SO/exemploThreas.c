#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI minhaThread(LPVOID lpParam) {
    printf("Executando a thread!\n");
    return 0;
}

int main() {
    HANDLE hThread;
    DWORD threadId;

    hThread = CreateThread(
        NULL,        // Atributos de segurança
        0,           // Tamanho da pilha (0 = padrão)
        minhaThread, // Função da thread
        NULL,        // Argumento para a função
        0,           // Flags de criação
        &threadId    // ID da thread
    );

    // Espera a thread terminar
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}