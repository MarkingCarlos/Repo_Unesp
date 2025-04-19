#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

typedef struct processosFinalizados
{
   char nomePrograma[50];
   int indentificadorPrograma;
   int prioridadePrograma;
   float tamanhoPrograma;
   int statusPrograma;
   char semafarosPrograma[50];

};

typedef struct programa
{
    char nomePrograma[50];
    int indentificadorPrograma;
    int prioridadePrograma;
    float tamanhoPrograma;
    
}Programa;

typedef struct comandos{
    char tipoExecucao[10];
    int tempoTrilha;
};

void InserirProgramaSintatico(Programa programaSintatico,FILE *arquivoPrograma,char LinhaLida[5][100]);

int main() {
    printf("Trabalho SO");

    FILE *arquivoPrograma;
    char LinhaLida[5][100];
    Programa programaSintatico;

    arquivoPrograma = fopen("../programa.txt", "rt");

    if (arquivoPrograma == NULL)
    {
        printf("\nProblemas na leitura");
    }

    InserirProgramaSintatico(programaSintatico,arquivoPrograma,LinhaLida);



    return 0;
}

void InserirProgramaSintatico(Programa programaSintatico,FILE *arquivoPrograma,char LinhaLida[5][100]){
    for (int i = 0; i < 5; i++)
    {
        fgets(LinhaLida[i],99,arquivoPrograma);
    }

    strcpy(programaSintatico.nomePrograma, LinhaLida[0]);
    programaSintatico.indentificadorPrograma = atoi(LinhaLida[1]) ;
    programaSintatico.prioridadePrograma = atoi(LinhaLida[2]) ;
    programaSintatico.tamanhoPrograma =  atof(LinhaLida[3]);;

    printf("\nO que esta na strut: \nNome: %s", programaSintatico.nomePrograma);
    printf("Indentificador: %d", programaSintatico.indentificadorPrograma);
    printf("\nPrioridade: %d", programaSintatico.prioridadePrograma);
    printf("\nTamanho: %.f", programaSintatico.tamanhoPrograma);
}