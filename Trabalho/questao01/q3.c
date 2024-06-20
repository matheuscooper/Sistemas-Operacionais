#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h> 

#define NUM_THREADS 4

typedef struct Registro
{
    int nota;
} Registro;

Registro registro; // é a mesma variável global do codigo dele, mas para o registro das notas 
// se nao tem controle entao nao precisa fazer condiçoes 

void *escritor(void*t){
    long tid = (long)t;
    printf("thread %dl de escrita lenda a nota\n", tid);
    sleep(2);
    int notaAtual = registro.nota;
    printf("thread &ld de escrita leu a nota %d\n", tid, notaAtual);
    int notaAtualizada = notaAtual + 10;
    printf("thread %ld de escrita leu a nota atualizada %d\n", tid, notaAtualizada);
    sleep(3);
    registro.nota = notaAtualizada; // aqui eu atualizo a nota no registro
    printf("thread %ld atualizou a nota para %d\n", tid, registro.nota);
    printf("Thread %ld acabou\n", tid);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    int rc;
    long t;

    // inicializa o registro
    registro.nota = 50;

    for (t = 0; t < NUM_THREADS; t++) {
        printf("Main: criando a thread %ld\n", t);
        rc = pthread_create(&thread[t], NULL, escritor, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Espera as threads terminarem
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(thread[t], NULL);
    }

    printf("Nota final no registro = %d\n", registro.nota);
    printf("Main: programa terminou\n");
    pthread_exit(NULL);
    return 0;
}