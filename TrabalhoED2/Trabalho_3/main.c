#include <stdio.h>
#include <stdlib.h>


#define ORDER 3

// Estrutura para representar um nó da árvore B+
typedef struct Node {
    int RNN;    
    int isLeaf; // Indica se o nó é uma folha
    int numKeys; // Número atual de chaves no nó
    int keys[ORDER - 1]; // Chaves armazenadas no nó
    struct Node *pointers[ORDER]; // Ponteiros para os filhos (ou próximo, no caso de folhas)
    struct Node *next; // Ponteiro para o próximo nó (apenas para folhas)
} Node;

struct Filmes
{
    char cod_filme[6];
    char titulo[50];
    char titulo_original[50];
    char diretor[32];
    int ano[4];
    char pais[30];
    int nota[1];
};


// Função para criar um novo nó
Node *createNode() {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->isLeaf = 1;
    newNode->numKeys = 0;
    newNode->next = NULL;
    for (int i = 0; i < ORDER - 1; i++) {
        newNode->keys[i] = 0;
        newNode->pointers[i] = NULL;
    }
    newNode->pointers[ORDER - 1] = NULL;
    return newNode;
}

// Função para inserir uma chave na árvore B+
Node *insert(Node *root, int key) {
    if (root == NULL) {
        Node *newNode = createNode();
        newNode->keys[0] = key;
        newNode->numKeys = 1;
        return newNode;
    }

    // TODO: Implementar a lógica de inserção

    return root;
}

// TODO: Implementar outras funções necessárias, como busca, exclusão, impressão, etc.

int MostarMenu(){
    int op = 0;
    printf("Bem-Vindo\nEscolha o que deseja fazer:\n");
    printf("1- Inserir filme\n");
    printf("2- Buscar Filme\n");
    printf("3- Excluir filme\n");
    scanf("%d",&op);
    return op;
}

int main() {
    Node *root = NULL;



    // Exemplo de inserção de chaves na árvore B+
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);

    // TODO: Adicionar mais casos de teste e funcionalidades

    // Liberar a memória alocada
    // TODO: Implementar uma função para liberar a memória usada pelos nós da árvore
}