#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "projetoSO.h"

#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))

pthread_t thread_leitura[3];
pthread_t thread_escrita;
//pthread_t thread_processamento[n];

parametros_thread *aloca_vetor_parametros(unsigned int tamanho) {
  
  parametros_thread *vetor;

  if (tamanho<=0)
    return NULL;

  if ((vetor = (parametros_thread *) malloc(sizeof(parametros_thread)*tamanho)) == NULL){
    printf("Não foi possível alocar o vetor. \n");
    exit(EXIT_FAILURE);
  }

  return vetor;
}

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

void* leitura_matriz(void *args) {

    register int tamanho;
    char* nome_arq;

    tamanho = ((parametros_thread *) args)->tam_matriz;
    nome_arq = ((parametros_thread *) args)->nome_arquivo;


    FILE* arq_matriz = abrir_arquivo(nome_arq, "r");

    int *matriz = aloca_matriz(tamanho, tamanho);
    
    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            fscanf(arq_matriz, "%d", &matriz[posicao(i, j, tamanho)]);
        }
    }

    fclose(arq_matriz);

    return ((void*) matriz);
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

/*
clock_t calculo_tempo(int (*funcao)(int*, register int)) {

    clock_t inicio, fim;
    double tempo_total;
    inicio = clock();
    (*funcao)(int*, register int);
    fim = clock() - inicio;
    tempo_total = ((double) fim)/CLOCKS_PER_SEC;
 
    return tempo_total;
}*/

int main(int argc, char *argv[]) {

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

/* *******************************PASSO 1******************************** */
    //fazer função pra isso
    parametros_thread *parametros_leitura = aloca_vetor_parametros(2);
    parametros_leitura[0].tam_matriz = T;
    parametros_leitura[0].nome_arquivo = arqA;
    parametros_leitura[1].tam_matriz = T;
    parametros_leitura[1].nome_arquivo = arqB;

    pthread_create(&thread_leitura[0], NULL, leitura_matriz, (void*)&parametros_leitura[0]);
    pthread_create(&thread_leitura[1], NULL, leitura_matriz, (void*)&parametros_leitura[1]);

    void* matrizA;
    void* matrizB;

    pthread_join(thread_leitura[1], &matrizA);
    pthread_join(thread_leitura[0], &matrizB);

/* *******************************PASSO 2******************************** */
    //crar thread de processamento
    int *matrizD = soma_matrizes(matrizA, matrizB, T);

/* *******************************PASSO 3 e 4******************************** */
    gravar_matriz(T, arqD, matrizD);

    //disparando matriz C + matriz D
    parametros_leitura[2].tam_matriz = T;
    parametros_leitura[2].nome_arquivo = arqC;

    pthread_create(&thread_leitura[2], NULL, leitura_matriz, (void*)&parametros_leitura[2]);

    void *matrizC;

    pthread_join(thread_leitura[2], &matrizC);

/* *******************************PASSO 5******************************** */
    int *matrizE = multiplica_matrizes(matrizC, matrizD, T);

/* *******************************PASSO 6******************************** */
    gravar_matriz(T, arqE, matrizE);
    printf("Redução: %d\n", reduz_matriz(matrizE, T));

    printf("Tempo soma: \n");
    printf("Tempo multiplicação: \n");
    //printf("Tempo redução: %.3ld\n", calculo_tempo(reduz_matriz));
    printf("Tempo total: \n");

    free(parametros_leitura);
    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}