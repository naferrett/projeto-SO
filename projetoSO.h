#ifndef PROJETOSO_H_
#define PROJETOSO_H_

typedef struct {
    int indice_inicio;
    int indice_final;
    int tam_matriz;
    char *nome_arquivo;
    int *matriz;
} parametros_thread;

parametros_thread *aloca_vetor_parametros(unsigned int);


void* leitura_matriz(void *args);



#endif
