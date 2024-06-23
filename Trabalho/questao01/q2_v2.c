#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <semaphore.h>

#define NUM_THREADS 8
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"


typedef struct Registro {
    int nota;
} Registro;

Registro registro;

sem_t writer;
pthread_mutex_t mutex; 
pthread_cond_t withoutWriter;

int writers = NUM_THREADS/2;

void *leitor(void *t) {
    long tid = (long)t;

    pthread_mutex_lock(&mutex);
    while (writers != NUM_THREADS/2)
    {
        pthread_cond_wait(&withoutWriter, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    
    printf("Thread %ld de leitura lendo a nota\n", tid);
    int notaLida = registro.nota;
    usleep(rand() % 1000 );
    printf("Thread %ld de leitura leu a nota %d\n", tid, notaLida);


    printf("Thread %ld acabou :)\n", tid);

    return NULL;
}

void *escritor(void *t) {
    long tid = (long)t;

    pthread_mutex_lock(&mutex);
    writers--;
    pthread_mutex_unlock(&mutex);

    printf("%sThread %ld de escrita esperando para escrever %s\n", KGRN,tid, KWHT);


    printf("%sThread %ld de escrita escrevendo a nota\n %s",KGRN, tid, KWHT);
    usleep(rand() % 2000 );
    sem_wait(&writer);
    int notaAtual = registro.nota;
    printf("%sThread %ld de escrita leu a nota %d %s\n",KGRN, tid, notaAtual, KWHT);
    int notaAtualizada = notaAtual + 6;
    printf("%sThread %ld de escrita atualizou a nota para %d %s\n", KGRN, tid, notaAtualizada, KWHT);
    usleep(rand() % 1000 );
    registro.nota = notaAtualizada;
    sem_post(&writer);
    printf("%sThread %ld atualizou a nota para %d %s\n", KGRN, tid, registro.nota, KWHT);



    printf("%sThread %ld acabou %s\n",KGRN, tid, KWHT);

    pthread_mutex_lock(&mutex);
    writers++;
    pthread_cond_broadcast(&withoutWriter);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    int rc;
    long t;

    sem_init(&writer, 0, 1);

    registro.nota = 50;

    int readThread = 0;
    int writerThreads = 0;
    int maxThreadsPerOperation = NUM_THREADS / 2;

    for (t = 0; t < NUM_THREADS; t++) {

        if ( ((rand() % 2 == 0 && readThread < maxThreadsPerOperation)) || writerThreads >= maxThreadsPerOperation) {
            printf("Main: criando a thread leitora %ld\n", t);
            rc = pthread_create(&thread[t], NULL, leitor, (void *)t);
            readThread++;
        } else {
            printf("Main: criando a thread escritora %ld\n", t);
            rc = pthread_create(&thread[t], NULL, escritor, (void *)t);
            writerThreads++;
        }

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) pthread_join(thread[t], NULL);

    printf("Nota final no registro = %d\n", registro.nota);
    printf("Main: programa terminou\n");
    pthread_exit(NULL);
    return 0;
}
