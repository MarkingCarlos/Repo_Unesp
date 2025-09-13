

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main() {
    FILE *file;
    char nome[10];
    int idade=0,numFilho=0, op;
    file = fopen("lista.txt","a");
    printf("Escreva o nome, idade e numero de filhos de 2 usuarios\n");
    printf("Desaja começar arquivo do 0?\n0-Nao\n1-Sim\n");
    scanf("%d",&op);
    if(op==1) {
        file = fopen("lista.txt","w");//abre o arquivo para escrita apagando o conteúdo
    }else {
        file = fopen("lista.txt","a");//abre para escrita sem apagar o conteúdo
    }


    for (int i=0;i<2;i++) {
        printf("Nome: ");
        scanf("%s",nome);
        printf("Idade:");
        scanf("%d",&idade);
        printf("Numero de filhos:");
        scanf("%d",&numFilho);
        fprintf(file,"%s",nome);
        fprintf(file,"%d",idade);
        fprintf(file,"%d",numFilho);
    }

    fclose(file);

    file = fopen("lista.txt","r");
    fscanf(file,"%s",nome);

    printf("Informacao do arquivo: %s\n",nome);

}