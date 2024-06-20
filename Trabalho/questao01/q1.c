#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

#define NUM_THREADS 4

typedef struct Registro
{
    int nota;
} Registro;

Registro registro; // é a mesma variável global do codigo dele, mas para o registro das notas 

void *leitor(void *t){
    int notaLida;
    long tid = (long)t;
    printf("Thread %ld de leitura lendo a nota...\n", tid);
    sleep(2);
    notaLida = registro.nota;
    printf("Thread %ld leu a nota = %d\n", tid, notaLida);
    printf("Thread %ld acabou\n", tid);
    pthread_exit(NULL); // aqui ele fecha a thread
}

void *escritor(void *t){
    int notaAtual;
    long tid = (long)t;
    printf("Thread %ld de escrita lendo a maldita nota...\n", tid);
    sleep(2);
    notaAtualizada = registro.nota;
    printf("Thread %ld leu a nota do arrombado = %d\n", tid, notaAtual);
    // agora precisa assim como no código da simulação de saldo do banco, fazer uma atualizaçao do valor
    int notaAtualizada = notaAtual + 8
    printf("Thread %ld de escrita atualizou a nota = %d\n", tid, notaAtualizada);
    sleep(3); // simulação de tempo de escrita de registro
    registro.nota = notaAtualizada; 
    printf("Thread %ld atualizou a nota para = %d\n", tid, registro.nota);
    printf("Thread %ld done.\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    int rc;
    long t;

    // iniciando os registros
    registro.nota = 50;

    for(t = 0; t < NUM_THREADS; t++) {
        printf("Main: criando as thread %ld\n", t);
        if (t % 2 == 0)
            rc = pthread_create(&thread[t], NULL, escritor, (void *)t);
        else
            rc = pthread_create(&thread[t], NULL, leitor, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Espera as threads terminarem
    for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(thread[t], NULL);
    }

    printf("Nota final no registro = %d\n", registro.nota);
    printf("Main: programa terminou\n");
    pthread_exit(NULL);
    return 0;
}