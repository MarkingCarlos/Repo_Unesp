#include "funcoes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



ArvoreB* CriarArvore(){
    ArvoreB* arvore = (ArvoreB*)malloc(sizeof(ArvoreB));
    arvore->raiz = criarNo();
    return arvore;
}



no * criarNo(){
    no* NovoNo = (no*)malloc(sizeof(no));
    NovoNo->numeroChaves = 0;
    NovoNo->IsFolha = 1; // folha

    for (int i = 0; i < ORDEM; i++)
    {
        NovoNo->RRNFilhos[i] = NULL;
        NovoNo->filhos[i] = NULL;
        strcpy(NovoNo->Placa[i], "\0");

       
    }

    return NovoNo;
    

}

int stringMaiorQue(char *str1, char *str2) {
    
    return strcmp(str1, str2);
}

int stringVazia(char *str) {
    if (str[0] == '\0')
    {
        return 1; //Verdade
    }else{
        return 0; //Falso
    }
    
}

 void INSERIR(no* arvore, char *placa){

    if (arvore->numeroChaves == 0)
    {
        strcpy(arvore->Placa[0],placa);
        arvore->numeroChaves = arvore->numeroChaves+1;
        arvore->RRN = 0;

    }else if(arvore->IsFolha == 1 ){

        if (arvore->numeroChaves == MAXCHAVES)
        {
          arvore =  Dividir(arvore,placa);
        }else{

            for (int i = 0; i < MAXCHAVES; i++)
            {
                if (stringMaiorQue(arvore->Placa[i],placa)<0 && (stringVazia(arvore->Placa[i+1]) == 1)) // se for menor
                {
                    strcpy(arvore->Placa[i+1],placa);
                    arvore->numeroChaves = arvore->numeroChaves+1;
                    return;
                }else if(stringMaiorQue(arvore->Placa[i],placa)>0){
                // printf("nao devia\n\n");
                    char PlacaRealocada[8];
                    strcpy(PlacaRealocada,arvore->Placa[i]);
                    strcpy(arvore->Placa[i],placa);
                    printf("Placa realocada: %s\n\n",PlacaRealocada);
                    INSERIR(arvore,PlacaRealocada);
                    return;
                    //arvore->raiz->numeroChaves = arvore->raiz->numeroChaves+1;
                }
            }

        }
        
  
        
    }
    
}

ArvoreB* Dividir(ArvoreB* arvore, char *placa){
    int ElementosEsquerda = (ORDEM / 2)-1;
    printf("resultado:%d  \n",ElementosEsquerda);
    printf("Chave que devera ser promovida: %s\n", arvore->raiz->Placa[ElementosEsquerda]);
    no* novaArvoreEsq = criarNo();
    no* novaArvoreDireira = criarNo();
    ArvoreB* ArvoreNova = CriarArvore();
    for (int i = 0; i < ElementosEsquerda; i++)
    {
        INSERIR(novaArvoreEsq,arvore->raiz->Placa[i]);
    }
    for (int i = MAXCHAVES; i > ElementosEsquerda; i--)
    {
        INSERIR(novaArvoreDireira,arvore->raiz->Placa[i]);
    }
    
    INSERIR(ArvoreNova,arvore->raiz->Placa[ElementosEsquerda]);

    ArvoreNova->raiz->filhos[0] = novaArvoreEsq;
    ArvoreNova->raiz->filhos[1] = novaArvoreDireira;
    return ArvoreNova;
    
    
}




void LerArquivo(){
    FILE *datFile = fopen("veiculos.dat", "rb");
    if (datFile == NULL) {
    perror("Erro ao abrir o arquivo veiculos.dat");
    return ;
    }
    size_t tamanho_registro = sizeof(Veiculo);
    printf("Tamanho de um registro em bytes: %zu\n", tamanho_registro);
    Veiculo veiculo;
    // Ler o registro
    size_t registros_lidos = fread(&veiculo, tamanho_registro, 1, datFile);
    if (registros_lidos != 1)
    {
        printf("Erro ao ler o registro");
        fclose(datFile);
        return ;
    }
    // Imprimir os dados do veículo
    printf("Registro de RNN 0\n");
    printf("Placa: %s\n", veiculo.placa);
    printf("Modelo: %s\n", veiculo.modelo);
    printf("Marca: %s\n", veiculo.marca);
    printf("Ano: %d\n", veiculo.ano);
    printf("Categoria: %s\n", veiculo.categoria);
    printf("Quilometragem: %d\n", veiculo.quilometragem);
    printf("Status: %s\n", veiculo.status);
    printf("---------------------------\n");
}