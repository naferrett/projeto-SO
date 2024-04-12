#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))

FILE *abrir_arquivo(char* nome_arq, char* modo) {
    FILE *arq = fopen(nome_arq, modo);

    if (arq == NULL) {
        printf ("Não foi possível abrir o arquivo.\n");
        exit(1);
    }
    return arq;
}

int *aloca_matriz(int linhas, int colunas) {
    int *matriz = (int *) malloc(linhas * colunas * sizeof(int));
    
    return matriz;
}

void gerar_matriz(register int tamanho, char *nome_arq) {

    FILE *arq_matriz;

    register int i, j;

    if(tamanho == 100 || tamanho == 1000) {
        arq_matriz = abrir_arquivo(nome_arq, "w");

        for(i = 0; i < 100; i++) {
            for(j = 0; j < 100; j++) {
                fprintf(arq_matriz, "%d ", 1);
            }

            fprintf(arq_matriz, "\n"); 
        }

        fclose(arq_matriz);
    } 
    else {
        printf("O tamanho inserido para a matriz não pode ser gerado.");
    }
}

int* leitura_matriz(register int tamanho, char* nome_arq) {

    FILE* arq_matriz = abrir_arquivo(nome_arq, "r");

    //alocação dinamica da matriz
    int *matriz = aloca_matriz(tamanho, tamanho);
    
    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            fscanf(arq_matriz, "%d", &matriz[posicao(i, j, tamanho)]);
        }
    }

    fclose(arq_matriz);

    return matriz;
}

void gravar_matriz(int tamanho, char* nome_arq, int* matriz) {

    FILE* arq_matriz = abrir_arquivo(nome_arq, "w");

    register int i, j;
    for(i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++) {
            fprintf(arq_matriz, "%d ", matriz[posicao(i, j, tamanho)]);
        }

        fprintf(arq_matriz, "\n"); 
    }

    fclose(arq_matriz);
}

int *soma_matrizes(int *matriz_1, int *matriz_2, register int tamanho) {

    int *matriz_soma = aloca_matriz(tamanho, tamanho);

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            matriz_soma[posicao(i, j, tamanho)] = matriz_1[posicao(i, j, tamanho)] + matriz_2[posicao(i, j, tamanho)];
        }
    }

    return matriz_soma;
}

int *multiplica_matrizes(int *matriz_1, int *matriz_2, register int tamanho) {

    int *matriz_mult = aloca_matriz(tamanho, tamanho);

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            matriz_mult[posicao(i, j, tamanho)] = matriz_1[posicao(i, j, tamanho)] * matriz_2[posicao(i, j, tamanho)];
        }
    }

    return matriz_mult;
}

int reduz_matriz(int *matriz, register int tamanho) {

    int valor_reducao = 0;

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            valor_reducao += matriz[posicao(i, j, tamanho)];
        }
    }

    return valor_reducao;

}

int main() {
    gerar_matriz(100, "arqA.dat");
    gerar_matriz(100, "arqB.dat");
    gerar_matriz(100, "arqC.dat");

    int *matrizA = leitura_matriz(100, "arqA.dat");
    int *matrizB = leitura_matriz(100, "arqB.dat");

    int *matrizD = soma_matrizes(matrizA, matrizB, 100);
    gravar_matriz(100, "arqD.dat", matrizD);

    int* matrizC =  leitura_matriz(100, "arqC.dat");

    int *matrizE = multiplica_matrizes(matrizC, matrizD, 100);
    gravar_matriz(100, "arqE.dat", matrizE);
    printf("red: %d", reduz_matriz(matrizE, 100));

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}