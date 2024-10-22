#include <stdio.h>
#include <stdlib.h>

#define Ordem 5

typedef struct Node
{
    int RRN;
    int isFolha;
    int numeroChave;
    int Chaves[Ordem -2];
    struct Node *Filhos[Ordem];
    struct Node *proximo;
    
    
}Node;

struct veiculo
{
    char placa[7];
    char modelo[10];
    char marca[10];
    char status[20];
    
};

Node *CriarNo(){
    Node *NovoNo = (Node *)malloc(sizeof(Node));
    NovoNo->isFolha = 1; // 1 - True
    NovoNo->numeroChave = 0;
    NovoNo->proximo= NULL;
    for (int i = 0; i < Ordem - 1; i++)
    {
        NovoNo->Chaves[i] = 0;
        NovoNo->Filhos[i] = NULL;
    }
    NovoNo->Filhos[Ordem] = NULL;
    return NovoNo;
    
}

// Declaração do protótipo da função Dividir
Node *Dividir(Node *ArvoreB, int chave);

Node *Inserir(Node *ArvoreB, int chave){
    if (ArvoreB == NULL)
    {
        Node *NovaArvoreB = CriarNo();
        NovaArvoreB->Chaves[0] = chave;
        NovaArvoreB->numeroChave = 1;
        return NovaArvoreB;
    }
    if(ArvoreB->numeroChave == 4){
         printf("Divide");
         ArvoreB = Dividir(ArvoreB,chave);
         return ArvoreB;
    }
    else{

        for (int i = 0; i < (Ordem - 2); i++)
        {
            if (chave > ArvoreB->Chaves[i])
            {
                if (ArvoreB->Chaves[i+1] == 0)
                {
                    ArvoreB->Chaves[i+1] = chave;
                 
                }
                              
            }else if(chave < ArvoreB->Chaves[i]){

                int ChaveRepo = ArvoreB->Chaves[i];
                ArvoreB->Chaves[i] = chave;
                
                return Inserir(ArvoreB,ChaveRepo);
            }
        }
        ArvoreB->numeroChave++;
        return ArvoreB;
        
    }
    
}

Node *Dividir(Node * ArvoreB, int chave){
    Node *NovaArvore = CriarNo();
    int meio = ArvoreB->numeroChave/2;
    int ChavePromovida = ArvoreB->Chaves[meio];

    //insere a chave promovida no novo nó
    NovaArvore = Inserir(NovaArvore,ChavePromovida);

    for (int i = 0; i < meio; i++)
    {
        NovaArvore = Inserir(NovaArvore->Filhos[0], ArvoreB->Chaves[i]);
    }
    for (int i = (meio + 1); i < (Ordem-1); i++)
    {
        NovaArvore = Inserir(NovaArvore->Filhos[1], ArvoreB->Chaves[i]);
    }
    

    //NovaArvore = Inserir(NovaArvore->Filhos[0], 1);
    //NovaArvore = Inserir(NovaArvore->Filhos[1], 3);
    //NovaArvore = Inserir(NovaArvore->Filhos[1], 3); 
    return NovaArvore;
    
}
int main (){
    printf("Incio\n");
    Node *Arvore = NULL;
    Arvore = Inserir(Arvore,1);
    Arvore = Inserir(Arvore,4);
    Arvore = Inserir(Arvore,3);
    Arvore = Inserir(Arvore,2);
    Arvore = Inserir(Arvore,5);

    printf("    Chave na pos 0: %d\n", Arvore->Filhos[0]->Chaves[0]);
    //printf("    Chave na pos 1: %d\n", Arvore->Chaves[1]);
    //printf("    Chave na pos 2: %d\n", Arvore->Chaves[2]);
    //printf("    Chave na pos 3: %d\n", Arvore->Chaves[3]);
   // printf("    Numero de Chaves: %d\n", Arvore->numeroChave);

    printf("Final");
}