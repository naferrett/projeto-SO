#include "funcs.h"

parametros_thread *aloca_vetor_parametros(unsigned int tamanho) {

    parametros_thread *vetor;

    if (tamanho <= 0)
        return NULL;

    vetor = (parametros_thread *) malloc(sizeof(parametros_thread)*tamanho);

    if (vetor == NULL){
        printf("Não foi possível alocar o vetor.\n");
        exit(EXIT_FAILURE);
    }

    return vetor;
}

FILE *abrir_arquivo(char* nome_arq, char* modo) {
    FILE *arq = fopen(nome_arq, modo);

    if (arq == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }
    return arq;
}

int *aloca_matriz(int linhas, int colunas) {
    int *matriz = (int *) malloc(linhas * colunas * sizeof(int));

    if(matriz == NULL) {
        printf("Não foi possível alocar a matriz.\n");
        exit(EXIT_FAILURE);
    }
    
    return matriz;
}

void verificar_qntd_thrds(int tamanho, int qntd_thrds) {

    if(tamanho % qntd_thrds != 0) {
        printf("A matriz não é divisível pela quantidade de threads.\n");
        exit(EXIT_FAILURE);
    }
}

void verificar_criacao_thrd(int thread) {
    if (thread != 0) {
        printf("Erro na criação das threads.\n");
        exit(EXIT_FAILURE);
    }

}

void verificar_juncao_thrd(int thread) {
    if (thread != 0) {
        printf("Erro na junção das threads.\n");
        exit(EXIT_FAILURE);
    }
}

void gerar_matriz(register int tamanho, char *nome_arq) {

    FILE *arq_matriz;

    register int i, j;

    if(tamanho == 100 || tamanho == 1000) {
        arq_matriz = abrir_arquivo(nome_arq, "w");

        for(i = 0; i < tamanho; i++) {
            for(j = 0; j < tamanho; j++) {
                fprintf(arq_matriz, "%d ", 1);
            }

            fprintf(arq_matriz, "\n"); 
        }

        fclose(arq_matriz);
    } 
    else {
        printf("O tamanho inserido para a matriz não pode ser gerado.\n");
        exit(EXIT_FAILURE);
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
    register int tamanho;
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
    return NULL;
}

void *soma_matrizes(void* args) {

    register int i, j, inicio, final, tamanho;
    int *matriz_1, *matriz_2, *matriz_soma;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz_1 = ((parametros_thread*)args)->matriz_1;
    matriz_2 = ((parametros_thread*)args)->matriz_2;
    matriz_soma = ((parametros_thread*)args)->matriz_final;

    for(i = inicio; i <= final; i++) {
        for(j = 0; j < tamanho; j++) {
            matriz_soma[posicao(i, j, tamanho)] = matriz_1[posicao(i, j, tamanho)] + matriz_2[posicao(i, j, tamanho)];
        }
    }

    return ((void*) matriz_soma);
}

void *multiplica_matrizes(void *args) {

    register int i, j, inicio, final, tamanho;
    int *matriz_1, *matriz_2, *matriz_mult;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz_1 = ((parametros_thread*)args)->matriz_1;
    matriz_2 = ((parametros_thread*)args)->matriz_2;
    matriz_mult = ((parametros_thread*)args)->matriz_final;

    for(i = inicio; i <= final; i++) {
        for(j = 0; j < tamanho; j++) {
            matriz_mult[posicao(i, j, tamanho)] = 0;
            for(register int k = 0; k < tamanho; k++) {
                matriz_mult[posicao(i, j, tamanho)] += matriz_1[posicao(i, k, tamanho)] * matriz_2[posicao(k, j, tamanho)];
            }
        }
    }

    //return NULL;
    return ((void*) matriz_mult);
}

void *reduz_matriz(void* args) {

    register int i, j, inicio, final, tamanho;
    int *matriz, *reducao_parcial;

    inicio = ((parametros_thread*)args)->indice_inicio;
    final = ((parametros_thread*)args)->indice_final;
    tamanho = ((parametros_thread*)args)->tam_matriz;
    matriz = ((parametros_thread*)args)->matriz_final;
    

    if ((reducao_parcial = (int*)malloc(sizeof(int))) == NULL) {
        fprintf(stderr, "Não foi possível alocar memória para a redução parcial.\n");
        exit(EXIT_FAILURE);
    }

    *reducao_parcial = 0;

    for(i = inicio; i <= final; i++) {
        for(j = 0; j < tamanho; j++) {
            *reducao_parcial += matriz[posicao(i, j, tamanho)];
        }
    }

    return ((void*) reducao_parcial);

}

//passos para n != 1
void* leitura_A_B(int T, char *arqA, char* arqB, int** matrizA, int** matrizB) {

    pthread_t thread_leitura[2];
    parametros_thread *parametros_leitura = aloca_vetor_parametros(2);

    parametros_leitura[0].tam_matriz = T;
    parametros_leitura[0].nome_arquivo = arqA;
    parametros_leitura[1].tam_matriz = T;
    parametros_leitura[1].nome_arquivo = arqB;

    int err;

    err = pthread_create(&thread_leitura[0], NULL, leitura_matriz, (void*)&parametros_leitura[0]);
    verificar_criacao_thrd(err);
    err = pthread_create(&thread_leitura[1], NULL, leitura_matriz, (void*)&parametros_leitura[1]);
    verificar_criacao_thrd(err);

    void *matriz_lida;
    err = pthread_join(thread_leitura[1], &matriz_lida);
    verificar_juncao_thrd(err);
    *matrizA = (int*)matriz_lida;

    err = pthread_join(thread_leitura[0], &matriz_lida);
    verificar_juncao_thrd(err);
    *matrizB = (int*) matriz_lida;

    free(parametros_leitura);
}

void* soma_A_B(int qntd_thrds, int T, int* matrizA, int* matrizB, int** matrizD) {
    register int i, qntd_por_thread;
    int err;

    pthread_t thread_processamento[qntd_thrds];
    parametros_thread *parametros_processamento = aloca_vetor_parametros(qntd_thrds);

    verificar_qntd_thrds(T, qntd_thrds);
    qntd_por_thread = T/qntd_thrds;

    for(i = 0; i < qntd_thrds; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_1 = matrizA;
        parametros_processamento[i].matriz_2 = matrizB;
        parametros_processamento[i].matriz_final = *matrizD;
        
        err = pthread_create(&thread_processamento[i], NULL, soma_matrizes, (void*) &parametros_processamento[i]);
        verificar_criacao_thrd(err);
    }

    for(i = 0; i < qntd_thrds; i++) {
        err = pthread_join(thread_processamento[i], NULL);
        verificar_juncao_thrd(err);
    }

    free(parametros_processamento);
}

void* gravar_D_ler_C(int T, char* arqC, char* arqD, int **matrizC, int* matrizD) {
    pthread_t thread_escrita;
    parametros_thread parametros_escrita;
    
    pthread_t thread_leitura;
    parametros_thread parametros_leitura;

    parametros_escrita.tam_matriz = T;
    parametros_escrita.nome_arquivo = arqD;
    parametros_escrita.matriz_final = matrizD;

    parametros_leitura.tam_matriz = T;
    parametros_leitura.nome_arquivo = arqC;

    int err;

    err = pthread_create(&thread_escrita, NULL, gravar_matriz, (void*)&parametros_escrita);
    verificar_criacao_thrd(err);

    err = pthread_create(&thread_leitura, NULL, leitura_matriz, (void*)&parametros_leitura);
    verificar_criacao_thrd(err);

    void *matriz_lida;
    err = pthread_join(thread_leitura, &matriz_lida);
    verificar_juncao_thrd(err);
    *matrizC = (int*)matriz_lida;

    err = pthread_join(thread_escrita, NULL);
    verificar_juncao_thrd(err);
}

void* multiplicar_C_D(int qntd_thrds, int T, int* matrizC, int* matrizD, int** matrizE) {
    register int i, qntd_por_thread;
    int err;

    pthread_t thread_processamento[qntd_thrds];
    parametros_thread *parametros_processamento = aloca_vetor_parametros(qntd_thrds);

    qntd_por_thread = T/qntd_thrds;

        for(i = 0; i < qntd_thrds; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_1 = matrizC;
        parametros_processamento[i].matriz_2 = matrizD;
        parametros_processamento[i].matriz_final = *matrizE;
        
        err = pthread_create(&thread_processamento[i], NULL, multiplica_matrizes, (void*) &parametros_processamento[i]);
        verificar_criacao_thrd(err);
    }

    for(i = 0; i < qntd_thrds; i++) {
        err = pthread_join(thread_processamento[i], NULL);
        verificar_juncao_thrd(err);
    }

    free(parametros_processamento);
}

int gravar_e_reduzir_E(int qntd_thrds, int T, char *arqE, int* matrizE) {
    register int i, qntd_por_thread;

    pthread_t thread_escrita;
    parametros_thread parametros_escrita;

    parametros_escrita.tam_matriz = T;
    parametros_escrita.nome_arquivo = arqE;
    parametros_escrita.matriz_final = matrizE;

    pthread_t thread_processamento[qntd_thrds];
    parametros_thread *parametros_processamento = aloca_vetor_parametros(qntd_thrds);

    verificar_qntd_thrds(T, qntd_thrds);
    qntd_por_thread = T/qntd_thrds;

    void* soma_parcial = NULL;
    long long int soma_total = 0;
    int err;
    
    err = pthread_create(&thread_escrita, NULL, gravar_matriz, (void*)&parametros_escrita);
    verificar_criacao_thrd(err);

    for(i = 0; i < qntd_thrds; i++) {
        parametros_processamento[i].indice_inicio = qntd_por_thread * i;
        parametros_processamento[i].indice_final = (qntd_por_thread * (i+1)) - 1;
        parametros_processamento[i].tam_matriz = T;
        parametros_processamento[i].matriz_final = matrizE;
        
        err = pthread_create(&thread_processamento[i], NULL, reduz_matriz, (void*) &parametros_processamento[i]);
        verificar_criacao_thrd(err);
    }

    for(i = 0; i < qntd_thrds; i++) {
        err = pthread_join(thread_processamento[i], &soma_parcial);
        verificar_juncao_thrd(err);
        soma_total += *((int *) soma_parcial);
        free(soma_parcial);
    }

    err = pthread_join(thread_escrita, NULL);
    verificar_juncao_thrd(err);

    free(parametros_processamento);
    return soma_total;
}

void multiplas_thrds_exe(parametros_de_exe *args, resultado_e_tempo *calculo) {
    clock_t inicio, fim;

    leitura_A_B(args->tamanho, args->arqA, args->arqB, &args->matrizA, &args->matrizB);

    inicio = clock();
    soma_A_B(args->qntd_thrds, args->tamanho, args->matrizA, args->matrizB, &args->matrizD);
    fim = clock() - inicio;
    calculo->tempo_soma = ((double) fim) / CLOCKS_PER_SEC;

    gravar_D_ler_C(args->tamanho, args->arqC, args->arqD, &args->matrizC, args->matrizD);

    inicio = clock();
    multiplicar_C_D(args->qntd_thrds, args->tamanho, args->matrizA, args->matrizD, &args->matrizE);
    fim = clock() - inicio;
    calculo->tempo_mult = ((double) fim) / CLOCKS_PER_SEC;

    inicio = clock();
    calculo->resultado_red = gravar_e_reduzir_E(args->qntd_thrds, args->tamanho, args->arqE, args->matrizE);
    fim = clock() - inicio;
    calculo->tempo_red = ((double) fim) / CLOCKS_PER_SEC;
}

//passos para n = 1
int* leitura_sem_threads(register int tamanho, char* nome_arq) {

    FILE* arq_matriz = abrir_arquivo(nome_arq, "r");

    int *matriz = aloca_matriz(tamanho, tamanho);

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            fscanf(arq_matriz, "%d", &matriz[posicao(i, j, tamanho)]);
        }
    }

    fclose(arq_matriz);

    return matriz;
}

void gravacao_sem_threads(int tamanho, char* nome_arq, int* matriz) {

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

int* soma_sem_threads(int *matriz_1, int *matriz_2, register int tamanho) {

    int *matriz_soma = aloca_matriz(tamanho, tamanho);

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            matriz_soma[posicao(i, j, tamanho)] = matriz_1[posicao(i, j, tamanho)] + matriz_2[posicao(i, j, tamanho)];
        }
    }

    return matriz_soma;
}

int* mult_sem_threads(int *matriz_1, int *matriz_2, register int tamanho) {

    int *matriz_mult = aloca_matriz(tamanho, tamanho);

    for (register int i = 0; i < tamanho; i++) {
        for (register int j = 0; j < tamanho; j++) {
            matriz_mult[posicao(i, j, tamanho)] = 0;
            for (register int k = 0; k < tamanho; k++) {
                matriz_mult[posicao(i, j, tamanho)] += matriz_1[posicao(i, k, tamanho)] * matriz_2[posicao(k, j, tamanho)];
            }
        }
    }

    return matriz_mult;
}

int reducao_sem_threads(int *matriz, register int tamanho) {

    int valor_reducao = 0;

    for(register int i = 0; i < tamanho; i++) {
        for(register int j = 0; j < tamanho; j++) {
            valor_reducao += matriz[posicao(i, j, tamanho)];
        }
    }

    return valor_reducao;

}

void* unica_thrd_exe(void *thrd_args) {
    parametros_thread_unica *novo_args = (parametros_thread_unica*) thrd_args;
    parametros_de_exe *args = novo_args->args;
    resultado_e_tempo *calculo = novo_args->calculo;

    clock_t inicio, fim;

    args->matrizA = leitura_sem_threads(args->tamanho, args->arqA);
    args->matrizB = leitura_sem_threads(args->tamanho, args->arqB);

    inicio = clock();
    args->matrizD = soma_sem_threads(args->matrizA, args->matrizB, args->tamanho);
    fim = clock() - inicio;
    calculo->tempo_soma = ((double) fim) / CLOCKS_PER_SEC;

    gravacao_sem_threads(args->tamanho, args->arqD, args->matrizD);

    args->matrizC =  leitura_sem_threads(args->tamanho, args->arqC);

    inicio = clock();
    args->matrizE = mult_sem_threads(args->matrizC, args->matrizD, args->tamanho);
    fim = clock() - inicio;
    calculo->tempo_mult = ((double) fim) / CLOCKS_PER_SEC;   

    gravacao_sem_threads(args->tamanho, args->arqE, args->matrizE);

    inicio = clock();
    calculo->resultado_red = reducao_sem_threads(args->matrizE, args->tamanho);
    fim = clock() - inicio;
    calculo->tempo_red = ((double) fim) / CLOCKS_PER_SEC; 
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    int T = atoi(argv[2]);

    char *arqA = argv[3];
    char *arqB = argv[4];
    char *arqC = argv[5];
    char *arqD = argv[6];
    char *arqE = argv[7];

    int *matrizA = aloca_matriz(T, T);
    int *matrizB = aloca_matriz(T, T);
    int *matrizC = aloca_matriz(T, T);
    int *matrizD = aloca_matriz(T, T);
    int *matrizE = aloca_matriz(T, T);

    gerar_matriz(T, arqA);
    gerar_matriz(T, arqB);
    gerar_matriz(T, arqC);

    double tempo_soma, tempo_mult, tempo_red;
    int reducao_resultado;

    parametros_de_exe args;
    args.tamanho = T;
    args.qntd_thrds = n;
    args.matrizA = matrizA;
    args.matrizB = matrizB;
    args.matrizC = matrizC;
    args.matrizD = matrizD;
    args.matrizE = matrizE;
    args.arqA = arqA;
    args.arqB = arqB;
    args.arqC = arqC;
    args.arqD = arqD;
    args.arqE = arqE;

    resultado_e_tempo calculo;
    calculo.tempo_mult = tempo_mult;
    calculo.tempo_red = tempo_red;
    calculo.tempo_soma = tempo_soma;
    calculo.resultado_red = reducao_resultado;

    if(n == 1) {
        parametros_thread_unica novo_args = {&args, &calculo};
        unica_thrd_exe(&novo_args);
    } else {
        multiplas_thrds_exe(&args, &calculo);
    }

    printf("Redução: %d\n", calculo.resultado_red);
    printf("Tempo soma: %f segundos.\n", calculo.tempo_soma);
    printf("Tempo multiplicação: %f segundos.\n", calculo.tempo_mult);
    printf("Tempo redução: %f segundos.\n", calculo.tempo_red);
    printf("Tempo total: %f segundos.\n", calculo.tempo_soma + calculo.tempo_mult + calculo.tempo_red);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}