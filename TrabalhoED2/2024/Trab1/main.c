#include <inttypes.h>
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

Node *Inserir(Node *ArvoreB, int chave);

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
    // Cria um novo nó que será o pai
    Node *NovaArvore = CriarNo();
    NovaArvore->isFolha = 0; // Será um nó interno

    // Cria os nós filhos esquerdo e direito
    Node *FilhoEsquerdo = CriarNo();
    Node *FilhoDireito = CriarNo();

    int meio = (Ordem - 1) / 2;
    int ChavePromovida = ArvoreB->Chaves[meio];

    // Copia as chaves para o filho esquerdo
    for (int i = 0; i < meio; i++) {
        FilhoEsquerdo->Chaves[i] = ArvoreB->Chaves[i];
        FilhoEsquerdo->numeroChave++;
    }

    // Copia as chaves para o filho direito
    for (int i = meio + 1; i < ArvoreB->numeroChave; i++) {
        FilhoDireito->Chaves[i - (meio + 1)] = ArvoreB->Chaves[i];
        FilhoDireito->numeroChave++;
    }

    // Configura o novo nó como pai dos filhos esquerdo e direito
    NovaArvore->Chaves[0] = ChavePromovida;
    NovaArvore->numeroChave = 1;
    NovaArvore->Filhos[0] = FilhoEsquerdo;
    NovaArvore->Filhos[1] = FilhoDireito;

    // Verifica onde inserir a nova chave
    if (chave > ChavePromovida) {
        Inserir(FilhoDireito, chave);
    } else {
        Inserir(FilhoEsquerdo, chave);
    }

    return NovaArvore;
}


Node *Inserir(Node *ArvoreB, int chave) {

    if (ArvoreB == NULL) {
        Node *NovaArvoreB = CriarNo();
        NovaArvoreB->Chaves[0] = chave;
        NovaArvoreB->numeroChave = 1;
        return NovaArvoreB;
    }

    // Se o nó for uma folha, insere a chave na posição correta
    if (ArvoreB->isFolha) {
        // Se o nó estiver cheio, precisa ser dividido
        if (ArvoreB->numeroChave == Ordem - 1) {
            return Dividir(ArvoreB, chave);
        }
        int i;
        for (i = ArvoreB->numeroChave - 1; i >= 0 && chave < ArvoreB->Chaves[i]; i--) {
            ArvoreB->Chaves[i + 1] = ArvoreB->Chaves[i];
        }
        ArvoreB->Chaves[i + 1] = chave;
        ArvoreB->numeroChave++;


        return ArvoreB;
    }

    // Se não for folha, localiza o filho apropriado para a inserção
    int i;
    for (i = 0; i < ArvoreB->numeroChave && chave > ArvoreB->Chaves[i]; i++){}

    // Insere a chave no filho
    ArvoreB->Filhos[i] = Inserir(ArvoreB->Filhos[i], chave);

    // Se o filho foi dividido, deve-se lidar com a promoção da chave
    if (ArvoreB->Filhos[i]->numeroChave == Ordem - 1) {
        Node *NovoFilho = Dividir(ArvoreB->Filhos[i], chave);
        // Promove a chave do novo filho para o nó atual
        for (int j = ArvoreB->numeroChave; j > i; j--) {
            ArvoreB->Chaves[j] = ArvoreB->Chaves[j - 1];
            ArvoreB->Filhos[j + 1] = ArvoreB->Filhos[j];
        }
        ArvoreB->Chaves[i] = NovoFilho->Chaves[0];
        ArvoreB->Filhos[i] = NovoFilho->Filhos[0];
        ArvoreB->Filhos[i + 1] = NovoFilho->Filhos[1];
        ArvoreB->numeroChave++;
    }

    return ArvoreB;
}



int main() {
    printf("Início\n");
    Node *Arvore = NULL;
    Arvore = Inserir(Arvore, 1);
    Arvore = Inserir(Arvore, 4);
    Arvore = Inserir(Arvore, 3);
    Arvore = Inserir(Arvore, 2);
    Arvore = Inserir(Arvore, 5);
    Arvore = Inserir(Arvore, 6);

     printf("    Chave na pos 0: %d\n", Arvore->Filhos[1]->Chaves[2]);
   // printf("    Chave na pos 0: %d\n", Arvore->Filhos[0]->Chaves[3]);
    printf("    Chave na pos 1: %d\n", Arvore->Chaves[0]);
    // // printf("    Chave na pos 2: %d\n", Arvore->Chaves[2]);
    // // printf("    Chave na pos 3: %d\n", Arvore->Chaves[3]);
    // printf("    Número de Chaves: %d\n", Arvore->numeroChave);

    printf("Final\n");
    return 0;
}
