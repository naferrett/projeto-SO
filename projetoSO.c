#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "projetoSO.h"

#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))

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

void* gravar_matriz(void *args) {

    int tamanho;
    char* nome_arq;
    int* matriz;

    tamanho = ((parametros_thread *) args)->tam_matriz;
    nome_arq = ((parametros_thread *) args)->nome_arquivo;
    matriz = ((parametros_thread *) args)->matriz_final;

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

void *soma_matrizes(void* args) {

    int inicio, final;
    int *matriz_1, *matriz_2, *matriz_soma, tamanho;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz_1 = ((parametros_thread*)args)->matriz_1;
    matriz_2 = ((parametros_thread*)args)->matriz_2;
    matriz_soma = ((parametros_thread*)args)->matriz_final;

    for(register int i = inicio; i < final; i++) {
        for(register int j = 0; j < tamanho; j++) {
            matriz_soma[posicao(i, j, tamanho)] = matriz_1[posicao(i, j, tamanho)] + matriz_2[posicao(i, j, tamanho)];
        }
    }

    //return ((void*) matriz_soma);
    return NULL 
    //pthread exit?
}

void *multiplica_matrizes(void *args) {

    register int i, inicio, final;
    int *matriz_1, *matriz_2, *matriz_mult, tamanho;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz_1 = ((parametros_thread*)args)->matriz_1;
    matriz_2 = ((parametros_thread*)args)->matriz_2;
    matriz_mult = ((parametros_thread*)args)->matriz_final;

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            matriz_mult[posicao(i, j, tamanho)] = 0;
            for(register int a = 0; a < x: a++) {
                matriz_mult[posicao(i, j, tamanho)] += matriz_1[posicao(i, a, tamanho)] * matriz_2[posicao(a, j, tamanho)];
            }
        }
    }

    return NULL
    //return ((void*) matriz_mult);
}

void *reduz_matriz(void* args) {

    register int i, inicio, final;
    int *matriz, tamanho;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz = ((parametros_thread*)args)->matriz_final;
    
    int valor_reducao = 0;

    for(register int i = inicio; i < final; i++) {
        for(register int j = inicio; j < final; j++) {
            valor_reducao += matriz[posicao(i, j, final)];
        }
    }

    return ((void*) valor_reducao);

}

//passos
void* leitura_A_B(int T, char *arqA, char* arqB, int* matrizA, int* matrizB) {

    parametros_leitura[0].tam_matriz = T;
    parametros_leitura[0].nome_arquivo = arqA;
    parametros_leitura[1].tam_matriz = T;
    parametros_leitura[1].nome_arquivo = arqB;

    pthread_create(&thread_leitura[0], NULL, leitura_matriz, (void*)&parametros_leitura[0]);
    pthread_create(&thread_leitura[1], NULL, leitura_matriz, (void*)&parametros_leitura[1]);

    pthread_join(thread_leitura[1], (void**)&matrizA);
    pthread_join(thread_leitura[0], (void**)&matrizB);

}

void* soma_A_B(int qntd_por_thread, int T, int* matrizA, int* matrizB, int* matrizD) {

    for(register int i = 0; i < n; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_1 = matrizA;
        parametros_processamento[i].matriz_2 = matrizB;
        parametros_processamento[i].matriz_final = matrizD;
        
        pthread_create(&thread_processamento[i], NULL, soma_matrizes, (void*) &parametros_processamento[i]);
    }

    for(register int i = 0; i < n; i++) {
        pthread_join(thread_processamento[i], NULL);
    }
}

void* gravar_D_ler_C(int T, char* arqC, char* arqD, int *matrizC, int* matrizD) {

    parametros_escrita[0].tam_matriz = T;
    parametros_escrita[0].nome_arquivo = arqD;
    parametros_escrita[0].matriz_final = matrizD;

    parametros_leitura[2].tam_matriz = T;
    parametros_leitura[2].nome_arquivo = arqC;

    pthread_create(&thread_escrita[0], NULL, gravar_matriz, (void*)&parametros_escrita[0]);
    pthread_create(&thread_leitura[2], NULL, leitura_matriz, (void*)&parametros_leitura[2]);

    pthread_join(thread_leitura[2], (void**)&matrizC);
    pthread_join(thread_escrita[0], NULL);

}

void* multiplicar_D_C(int qntd_por_thread, int T, int* matrizC, int* matrizD, int* matrizE) {

        for(register int i = 0; i < n; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_1 = matrizD;
        parametros_processamento[i].matriz_2 = matrizC;
        parametros_processamento[i].matriz_final = matrizE;
        
        pthread_create(&thread_processamento[i], NULL, multiplica_matrizes, (void*) &parametros_processamento[i]);
    }

    for(register int i = 0; i < n; i++) {
        pthread_join(thread_processamento[i], NULL);
    }
}

void* gravar_e_reduzir_E(int qntd_por_thread, int T, char *arqE, int* matrizE) {
    //nao ta simultaneo essa bostaaaaaaaaaaaaa
    //gravar_matriz(T, arqE, matrizE);
    parametros_escrita[1].tam_matriz = T;
    parametros_escrita[1].nome_arquivo = arqE;
    parametros_escrita[1].matriz_final = matrizE;

    pthread_create(&thread_escrita[0], NULL, gravar_matriz, (void*)&parametros_escrita[0]);
    pthread_join(thread_escrita[1], NULL);

    //reduz_matriz(matrizE, T)
    void* soma = NULL;
    int soma_total = 0;

    for(int i = 0; i < n; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_final = matrizE;
        
        pthread_create(&thread_processamento[i], NULL, reduz_matriz, (void*) &parametros_processamento[i]);
    }

    for(int i = 0; i < n; i++) {
        pthread_join(thread_processamento[i], &soma);
        soma_total += *((int *) soma);
        free(soma);
    }

}

clock_t calculo_tempo(void (*funcao)(void*)) {

    clock_t inicio, fim;
    double tempo;
    inicio = clock();
    (*funcao)(NULL);
    fim = clock() - inicio;
    tempo = ((double) fim)/CLOCKS_PER_SEC;
 
    return tempo;
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    int T = atoi(argv[2]);

    char *arqA = argv[3];
    char *arqB = argv[4];
    char *arqC = argv[5];
    char *arqD = argv[6];
    char *arqE = argv[7];

    int qntd_por_thread = T/n;

    //acho que esse aviso não é necessario
    if(qntd_por_thread % n != 0) {
        printf("A matriz não é divisível pela quantidade de threads.");
        return 0;
    }

    int *matrizA = aloca_matriz(T, T);
    int *matrizB = aloca_matriz(T, T);
    int *matrizC = aloca_matriz(T, T);
    int *matrizD = aloca_matriz(T, T);
    int *matrizE = aloca_matriz(T, T);

    gerar_matriz(T, arqA);
    gerar_matriz(T, arqB);
    gerar_matriz(T, arqC);

    //declaração tem q ser aqui?
    pthread_t thread_leitura[3];
    pthread_t thread_escrita[2];
    pthread_t thread_processamento[n];

    //tem q ser aqui?
    parametros_thread *parametros_leitura = aloca_vetor_parametros(3);
    parametros_thread *parametros_escrita = aloca_vetor_parametros(2);
    parametros_thread *parametros_processamento = aloca_vetor_parametros(n);

/* *******************************IMPRESSÃO******************************** */
    printf("Redução: %d\n", 1);
    printf("Tempo soma: \n");
    printf("Tempo multiplicação: \n");
    printf("Tempo redução: \n");
    printf("Tempo total: \n");

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);
    free(parametros_leitura);
    free(parametros_escrita);
    free(parametros_processamento);

    return 0;
}