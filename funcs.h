#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef FUNCS_H_
#define FUNCS_H_

typedef struct {
    int indice_inicio;
    int indice_final;
    int tam_matriz;
    char *nome_arquivo;
    int *matriz_1;
    int *matriz_2;
    int *matriz_final;
} parametros_thread;

parametros_thread *aloca_vetor_parametros(unsigned int tamanho);

FILE *abrir_arquivo(char* nome_arq, char* modo);

int *aloca_matriz(int linhas, int colunas);

void verificar_qntd_thrds(int tamanho, int qntd_thrds);

void verificar_criacao_thrd(int thread);

void verificar_juncao_thrd(int thread);

void gerar_matriz(register int tamanho, char *nome_arq);

void* leitura_matriz(void *args);

void* gravar_matriz(void *args);

void *soma_matrizes(void* args);

void *multiplica_matrizes(void *args);

void *reduz_matriz(void* args);

void* leitura_A_B(int T, char *arqA, char* arqB, int** matrizA, int** matrizB);

void* soma_A_B(int qntd_thrds, int T, int* matrizA, int* matrizB, int** matrizD);

void* gravar_D_ler_C(int T, char* arqC, char* arqD, int **matrizC, int* matrizD);

void* multiplicar_C_D(int qntd_thrds, int T, int* matrizC, int* matrizD, int** matrizE);

int gravar_e_reduzir_E(int qntd_thrds, int T, char *arqE, int* matrizE);

#endif
