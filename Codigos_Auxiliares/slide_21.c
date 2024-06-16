#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 6

int saldo = 1856;

void* saque(void* arg) {

    int valor = 10;

    int saldo_local = saldo;
    sleep((rand() % 8) + 1);
    saldo_local -= valor;
    printf("[THREAD %ld](SAQUE) ==> Saque: %d, valor final: %d\n", (long) arg, valor, saldo_local);

    saldo = saldo_local;

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

    printf("main: end %d\n", saldo);

    return 0;
}
