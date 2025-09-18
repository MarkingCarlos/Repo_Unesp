#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


#define tamanhoMatriz 10
#define TOL 1e-5
#define MAX_IT 10000

void lerMatriz(FILE *arquivo, float Matriz[tamanhoMatriz][tamanhoMatriz], float vetorCoeficientes[tamanhoMatriz]) {
    for (int i = 0; i < tamanhoMatriz; i++)
        for (int j = 0; j < tamanhoMatriz; j++)
            fscanf(arquivo, "%f", &Matriz[i][j]);
    for (int i = 0; i < tamanhoMatriz; i++)
        fscanf(arquivo, "%f", &vetorCoeficientes[i]);
}

void escreveSaida(const char *nomeArquivo, float x[tamanhoMatriz]) {
    FILE *arquivoResultado = fopen(nomeArquivo, "w");
    if (!arquivoResultado) {
        perror("Erro ao criar arquivo de saída");
        exit(1);
    }
    for (int i = 0; i < tamanhoMatriz; i++)
        fprintf(arquivoResultado, "%.4f%c", x[i], (i == tamanhoMatriz-1) ? '\n' : ' ');
    fclose(arquivoResultado);
}

void metodoJacobi(float A[tamanhoMatriz][tamanhoMatriz], float b[tamanhoMatriz], float x[tamanhoMatriz]) {

    float novoX[tamanhoMatriz], diferenca, maiorDiferenca;
    int it = 0;
    #pragma omp parallel for
    for (int i = 0; i < tamanhoMatriz; i++) x[i] = 0;

    do {
        maiorDiferenca = 0.0;
        for (int i = 0; i < tamanhoMatriz; i++) {
            float soma = 0.0;
            for (int j = 0; j < tamanhoMatriz; j++) {
                if (j != i)
                    soma += A[i][j] * x[j];
            }
            novoX[i] = (b[i] - soma) / A[i][i];
        }
        // Calcula o maior deslocamento em relação à última iteração
        for (int i = 0; i < tamanhoMatriz; i++) {
            diferenca = fabs(novoX[i] - x[i]);

            if (diferenca > maiorDiferenca) {
                maiorDiferenca = diferenca;
            }

            x[i] = novoX[i];
        }
        it++;
    } while (maiorDiferenca > TOL && it < MAX_IT);

    printf("Iteracoes realizadas: %d\n", it);
}

int main() {
        float Matriz[tamanhoMatriz][tamanhoMatriz], vetorCoeficientes[tamanhoMatriz], x[tamanhoMatriz];
        FILE *arquivo = fopen("../linear10.dat", "r");

        if (!arquivo) {
            perror("Erro ao abrir arquivo de entrada");
            return 1;
        }

        lerMatriz(arquivo, Matriz, vetorCoeficientes);
        fclose(arquivo);

        double inicio = omp_get_wtime();

        metodoJacobi(Matriz, vetorCoeficientes, x);

        double fim = omp_get_wtime();

        printf("Tempo de execucao: %.6f segundos\n", fim - inicio);

        escreveSaida("saida.txt", x);

        return 0;

}
