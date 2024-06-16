#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 6

int saldo = 1856;
pthread_mutex_t mutex;

void* saque(void* arg) {
    int valor = 10;

    pthread_mutex_lock(&mutex);
    int saldo_local = saldo;
    sleep((rand() % 8) + 1);
    saldo_local -= valor;
    printf("[THREAD %ld](SAQUE) ==> Saque: %d, valor final: %d\n", (long) arg, valor, saldo_local);
    saldo = saldo_local;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* deposito(void* arg) {
    int valor = 15;

    int saldo_local = saldo;
    sleep((rand() % 15) + 1);
    saldo_local += valor;
    printf("[THREAD %ld](DEPOSITO) ==> Deposito: %d, valor final: %d\n", (long) arg, valor, saldo_local);
    saldo = saldo_local;


    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    pthread_mutex_init(&mutex, NULL);  // Initialize the mutex

    printf("main: begin\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        if (i % 2 == 0) {
            rc = pthread_create(&threads[i], NULL, saque, (void*)(long) i); 
            assert(rc == 0);
        } else {
            rc = pthread_create(&threads[i], NULL, deposito, (void*)(long) i); 
            assert(rc == 0);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL); 
        assert(rc == 0);
    }

    pthread_mutex_destroy(&mutex);  // Destroy the mutex

    printf("main: end %d\n", saldo);

    return 0;
}
