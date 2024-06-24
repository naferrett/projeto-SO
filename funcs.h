#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef FUNCS_H_
#define FUNCS_H_

//Responsável por realizar os acesso aos elementos das matrizes alocadas em uma única etapa
#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))

//Define os parâmetros que podem ser passados para uma thread.
typedef struct {
    int indice_inicio;
    int indice_final;
    int tam_matriz;
    char *nome_arquivo;
    int *matriz_1;
    int *matriz_2;
    int *matriz_final;
} parametros_thread;

//Define os parâmetros de execução do programa
typedef struct {
    int tamanho;
    int qntd_thrds;
    int *matrizA;
    int *matrizB;
    int *matrizC;
    int *matrizD;
    int *matrizE;
    char *arqA;
    char *arqB;
    char *arqC;
    char *arqD;
    char *arqE;
} parametros_de_exe;

//Define tempos de execução e resultado da redução do programa
typedef struct {
    double tempo_soma;
    double tempo_mult;
    double tempo_red;
    int resultado_red;
} resultado_e_tempo;

//Declaração de todas as funções usadas pelo programa
parametros_thread *aloca_vetor_parametros(unsigned int tamanho);

FILE *abrir_arquivo(char* nome_arq, char* modo);

int *aloca_matriz(int linhas, int colunas);

void verificar_qntd_thrds(int tamanho, int qntd_thrds);

void verificar_criacao_thrd(int thread);

void verificar_juncao_thrd(int thread);

void gerar_matriz(register int tamanho, char *nome_arq);

void *leitura_matriz(void *args);

void *gravar_matriz(void *args);

void *soma_matrizes(void* args);

void *multiplica_matrizes(void *args);

void *reduz_matriz(void* args);

void *leitura_A_B(int T, char *arqA, char* arqB, int** matrizA, int** matrizB);

void *soma_A_B(int qntd_thrds, int T, int* matrizA, int* matrizB, int** matrizD);

void *gravar_D_ler_C(int T, char* arqC, char* arqD, int **matrizC, int* matrizD);

void *multiplicar_C_D(int qntd_thrds, int T, int* matrizC, int* matrizD, int** matrizE);

int gravar_e_reduzir_E(int qntd_thrds, int T, char *arqE, int* matrizE, double* tempo_total);

void multiplas_thrds_exe(parametros_de_exe *args, resultado_e_tempo *calculo);

int *leitura_sem_threads(register int tamanho, char* nome_arq);

void gravacao_sem_threads(int tamanho, char* nome_arq, int* matriz);

int *soma_sem_threads(int *matriz_1, int *matriz_2, register int tamanho);

int *mult_sem_threads(int *matriz_1, int *matriz_2, register int tamanho);

int reducao_sem_threads(int *matriz, register int tamanho);

void* unica_thrd_exe(parametros_de_exe *args, resultado_e_tempo *calculo);

#endif
