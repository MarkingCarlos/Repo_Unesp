#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcoes.h"



int main (){
    ArvoreB * arvore = CriarArvore();
    INSERIR(arvore->raiz,"GIA5915");
    INSERIR(arvore->raiz,"VMV8316"); 
    INSERIR(arvore->raiz,"EEU1131"); 
    INSERIR(arvore->raiz,"VCI5034"); 
   // INSERIR(arvore,"ZCI5034");

    
    //LerArquivo();
    printf("\nPlaca 0: %s", arvore->raiz->Placa[0]);
    printf("\nPlaca 1: %s", arvore->raiz->Placa[1]);
    printf("\nPlaca 2: %s", arvore->raiz->Placa[2]);    
    printf("\nPlaca 3: %s", arvore->raiz->Placa[3]);

}