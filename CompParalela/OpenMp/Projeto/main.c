#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define quantidadeMatriz 2
void popularMatriz_Vetor(float Matriz[quantidadeMatriz][quantidadeMatriz], FILE *arquivo, float VetorResultados[quantidadeMatriz]);
void popularCom(float VetorX[quantidadeMatriz], float numero);
void metodoJacobi(float Matriz[quantidadeMatriz][quantidadeMatriz], float vetorCoeficientes[quantidadeMatriz], float vetorIteracaoAtual[quantidadeMatriz],int auxNumInte);
void transferirVetor(float VetorRecebe[quantidadeMatriz], float VetorEnvia[quantidadeMatriz]);
int main(void) {
    FILE *arquivo;
    float Matriz[quantidadeMatriz][quantidadeMatriz];
    float vetorCoeficientes[quantidadeMatriz], vetorIteracaoAtual[quantidadeMatriz];
    arquivo = fopen("../linear2.dat", "r");

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
                if (auxNumInte == 1) {
                    // printf("vetorAux[%d] = %.5f + (%.5f * %.5f)\n",
                    //    iLinha,
                    //    vetorAux[iLinha],
                    //    Matriz[iLinha][indice],
                    //    vetorIteracaoAtual[indice]);
                    // getchar();
                }

                vetorAux[iLinha] = vetorAux[iLinha] - Matriz[iLinha][indice] * (vetorIteracaoAtual[indice]);


            }
        }
        // if (auxNumInte == 1) {
        //     printf("vetor Atual Linha 0[] = %.5f \n",vetorIteracaoAtual[0]);
        //     printf("vetorAux[%d] = %.5f + %.5f\n",
        //    iLinha,
        //    vetorAux[iLinha],
        //    vetorCoeficientes[iLinha]);
        //     getchar();
        //
        // }

        vetorAux[iLinha]= vetorAux[iLinha] + vetorCoeficientes[iLinha];

        // if (auxNumInte == 1) {
        //     printf("vetorAux[%d] = %.5f",iLinha, vetorAux[iLinha]);
        //     getchar();
        // }
        //
        // if (auxNumInte == 1) {
        //     printf("vetorAux[%d] = %.5f / %.5f\n",
        //    iLinha,
        //    vetorAux[iLinha],
        //    Matriz[iLinha][iColuna]);
        //     getchar();
        //
        // }


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
            // printf("A diferença é maior ou igual a 1e-5\n");
            parada = 0;

        }
    }

    if (parada == 0) {

        // printf("VetorAux 0: %.4f \n", vetorAux[1]);

        // popularCom(vetorIteracaoAtual,vetorAux[0]);
        transferirVetor(vetorIteracaoAtual,vetorAux);

        // printf("Vetor Atual dps de atualizar linha 0 %.4f \n", vetorIteracaoAtual[1]);
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
    //Imprime Matriz
    // for (indice_linha = 0; indice_linha < 11; indice_linha++) {
    //     if (indice_linha == 10) {
    //         for (indice_coluna = 0 ; indice_coluna < 10; indice_coluna++) {
    //             printf("Vetor[%d]: %.4f\n", indice_coluna, VetorResultados[indice_coluna]);
    //         }
    //     }else {
    //         for (indice_coluna = 0 ; indice_coluna < 10; indice_coluna++) {
    //             printf("Matriz[%d][%d]: %.4f\n",indice_linha, indice_coluna, Matriz[indice_linha][indice_coluna]);
    //         }
    //     }
    //
    // }

}