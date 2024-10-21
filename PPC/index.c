#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib") // Link com a biblioteca Winsock

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;

    // Inicializa o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Falha na inicialização do Winsock. Código de erro: %d\n", WSAGetLastError());
        return 1;
    }

    // Cria o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Falha ao criar o socket. Código de erro: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configura o endereço do servidor
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço IP do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(8080); // Porta do servidor

    // Conecta ao servidor
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Falha na conexão. Código de erro: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Conectado ao servidor!\n");

    // Fecha o socket
    closesocket(sock);

    // Finaliza o Winsock
    WSACleanup();

    return 0;
}
