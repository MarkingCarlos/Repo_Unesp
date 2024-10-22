#include <stdio.h>
#include <stdlib.h>

#define Ordem 5

typedef struct Node {
    int RRN;
    int isFolha;
    int numeroChave;
    int Chaves[Ordem - 1];
    struct Node *Filhos[Ordem];
    struct Node *proximo;
} Node;

// Declaração do protótipo da função Dividir
Node *Dividir(Node *ArvoreB, int chave);

Node *CriarNo() {
    Node *NovoNo = (Node *)malloc(sizeof(Node));
    NovoNo->isFolha = 1; // 1 - True
    NovoNo->numeroChave = 0;
    NovoNo->proximo = NULL;
    for (int i = 0; i < Ordem - 1; i++) {
        NovoNo->Chaves[i] = 0;
        NovoNo->Filhos[i] = NULL;
    }
    NovoNo->Filhos[Ordem - 1] = NULL;
    return NovoNo;
}

Node *Dividir(Node *ArvoreB, int chave) {
    // Implementação da função Dividir
    Node *NovaArvore = CriarNo();
    int meio = ArvoreB->numeroChave / 2;
    int ChavePromovida = ArvoreB->Chaves[meio];

    // Configura a nova árvore
    NovaArvore->isFolha = ArvoreB->isFolha;
    NovaArvore->numeroChave = 1;
    NovaArvore->Chaves[0] = ChavePromovida;

    // Divide as chaves e filhos
    Node *FilhoEsquerdo = CriarNo();
    Node *FilhoDireito = CriarNo();

    for (int i = 0; i < meio; i++) {
        FilhoEsquerdo->Chaves[i] = ArvoreB->Chaves[i];
        FilhoEsquerdo->numeroChave++;
    }

    for (int i = meio + 1; i < ArvoreB->numeroChave; i++) {
        FilhoDireito->Chaves[i - meio - 1] = ArvoreB->Chaves[i];
        FilhoDireito->numeroChave++;
    }

    NovaArvore->Filhos[0] = FilhoEsquerdo;
    NovaArvore->Filhos[1] = FilhoDireito;

    return NovaArvore;
}

Node *Inserir(Node *ArvoreB, int chave) {
    if (ArvoreB == NULL) {
        Node *NovaArvoreB = CriarNo();
        NovaArvoreB->Chaves[0] = chave;
        NovaArvoreB->numeroChave = 1;
        return NovaArvoreB;
    }
    if (ArvoreB->numeroChave == Ordem - 1) {
        printf("Divide\n");
        ArvoreB = Dividir(ArvoreB, chave);
        return ArvoreB;
    } else {
        int i;
        for (i = ArvoreB->numeroChave - 1; i >= 0 && chave < ArvoreB->Chaves[i]; i--) {
            ArvoreB->Chaves[i + 1] = ArvoreB->Chaves[i];
        }
        ArvoreB->Chaves[i + 1] = chave;
        ArvoreB->numeroChave++;
        return ArvoreB;
    }
}

int main() {
    printf("Início\n");
    Node *Arvore = NULL;
    Arvore = Inserir(Arvore, 1);
    Arvore = Inserir(Arvore, 4);
    Arvore = Inserir(Arvore, 3);
    Arvore = Inserir(Arvore, 2);
    Arvore = Inserir(Arvore, 5);

    printf("    Chave na pos 0: %d\n", Arvore->Chaves[0]);
    // printf("    Chave na pos 1: %d\n", Arvore->Chaves[1]);
    // printf("    Chave na pos 2: %d\n", Arvore->Chaves[2]);
    // printf("    Chave na pos 3: %d\n", Arvore->Chaves[3]);
    printf("    Número de Chaves: %d\n", Arvore->numeroChave);

    printf("Final\n");
    return 0;
}
