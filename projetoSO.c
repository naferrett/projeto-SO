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
        exit(1);
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

int main(int argc, char *argv[]) {

    //Transformando de string para inteiro
    int n = atoi(argv[1]);
    int T = atoi(argv[2]);

    char *arqA = argv[3];
    char *arqB = argv[4];
    char *arqC = argv[5];
    char *arqD = argv[6];
    char *arqE = argv[7];

    gerar_matriz(T, arqA);
    gerar_matriz(T, arqB);
    gerar_matriz(T, arqC);

    int *matrizA = leitura_matriz(T, arqA);
    int *matrizB = leitura_matriz(T, arqB);

    int *matrizD = soma_matrizes(matrizA, matrizB, T);
    gravar_matriz(T, arqD, matrizD);

    int* matrizC =  leitura_matriz(T, arqC);

    int *matrizE = multiplica_matrizes(matrizC, matrizD, T);
    gravar_matriz(T, arqE, matrizE);
    printf("Matriz reduzida: %d", reduz_matriz(matrizE, T));

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}