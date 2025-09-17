#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define quantidadeMatriz 10
void popularMatriz_Vetor(float Matriz[quantidadeMatriz][quantidadeMatriz], FILE *arquivo, float VetorResultados[quantidadeMatriz]);
void popularCom(float VetorX[quantidadeMatriz], float numero);
void metodoJacobi(float Matriz[quantidadeMatriz][quantidadeMatriz], float vetorCoeficientes[quantidadeMatriz], float vetorIteracaoAtual[quantidadeMatriz],int auxNumInte);
void transferirVetor(float VetorRecebe[quantidadeMatriz], float VetorEnvia[quantidadeMatriz]);

int main(void) {
    FILE *arquivo;
    float Matriz[quantidadeMatriz][quantidadeMatriz];
    float vetorCoeficientes[quantidadeMatriz], vetorIteracaoAtual[quantidadeMatriz];
    arquivo = fopen("../linear10.dat", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    popularMatriz_Vetor(Matriz, arquivo, vetorCoeficientes);
    popularCom(vetorIteracaoAtual,0);
    metodoJacobi(Matriz,vetorCoeficientes,vetorIteracaoAtual,0);


    fclose(arquivo);
    return 0;
}


void metodoJacobi(float Matriz[quantidadeMatriz][quantidadeMatriz], float vetorCoeficientes[quantidadeMatriz], float vetorIteracaoAtual[quantidadeMatriz], int auxNumInte) {
    float vetorAux[quantidadeMatriz];
    int iColuna = 0;
    for (int i = 0; i < quantidadeMatriz; i++) {
        popularCom(vetorAux,0);
    }

    for (int iLinha = 0; iLinha < quantidadeMatriz; iLinha++) {
        for (int indice = 0; indice < quantidadeMatriz; indice++) {
            if (iLinha == indice) {

            }
            else {
                vetorAux[iLinha] = vetorAux[iLinha] - Matriz[iLinha][indice] * (vetorIteracaoAtual[indice]);
            }
        }

        // Pega o valor do X que foi isolado e passa para o outro lado
        vetorAux[iLinha]= vetorAux[iLinha] + vetorCoeficientes[iLinha];

        vetorAux[iLinha] = vetorAux[iLinha]/Matriz[iLinha][iColuna];


        printf("Valor X%d = %.4f\n",iLinha, vetorAux[iLinha]);;
        iColuna++;

    }

    float diferenca  = 1e-5;
    int parada = 1;
    for (int i = 0; i < quantidadeMatriz; i++) {
        if (fabs(vetorIteracaoAtual[i] - vetorAux[i]) < diferenca) {
            printf("A diferença entre é menor que 1e-5\n");
            parada = 1;
        } else {

            parada = 0;

        }
    }

    if (parada == 0) {

        transferirVetor(vetorIteracaoAtual,vetorAux);
        metodoJacobi(Matriz, vetorCoeficientes,vetorIteracaoAtual,1);
    }else {
        return;
    }

}

void popularCom(float VetorX[quantidadeMatriz], float numero) {
    for (int indice = 0 ; indice < quantidadeMatriz; indice++) {
        VetorX[indice] = numero;
    }
}

void transferirVetor(float VetorRecebe[quantidadeMatriz], float VetorEnvia[quantidadeMatriz]) {
    for (int indice = 0 ; indice < quantidadeMatriz; indice++) {
        VetorRecebe[indice] = VetorEnvia[indice];
    }
}


void popularMatriz_Vetor(float Matriz[quantidadeMatriz][quantidadeMatriz], FILE *arquivo, float VetorResultados[quantidadeMatriz]) {
    int indice_coluna = 0;
    int indice_linha = 0;
    for (indice_linha = 0; indice_linha < quantidadeMatriz+1; indice_linha++) {
        if (indice_linha == quantidadeMatriz) {
            for (indice_coluna = 0 ; indice_coluna < quantidadeMatriz; indice_coluna++) {
                fscanf(arquivo, "%f", &VetorResultados[indice_coluna]);
            }
        }else {
            for (indice_coluna = 0 ; indice_coluna < quantidadeMatriz; indice_coluna++) {
                fscanf(arquivo, "%f", &Matriz[indice_linha][indice_coluna]);
            }
        }


    }

}