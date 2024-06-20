#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

#define BUFFER_SIZE 5
#define CONSUMERS 1
#define PRODUCERS 5

typedef struct Queue {
    int buffer[BUFFER_SIZE];
    int front;
    int back;
    int size;
} Queue;

Queue queue;
sem_t empty;
sem_t full;

sem_t critical_region;

int produce(int threadID) {
    /* Insere um valor no final da fila */
    int item = rand() % 10000;
    queue.buffer[queue.back] = item;
    printf("%s[THREAD %d] ==> produced: %d\n%s", KGRN, threadID, item, KWHT);
    usleep(rand() % 1000 ); // Faz uma breve interrupção aleatoria
    queue.back = (queue.back + 1) % BUFFER_SIZE;
    queue.size += 1;
    return item;
}

void consume(int threadID) {
    /* Consome a cabeça da fila */
    if(queue.size <= 0 ) return;

    int item = queue.buffer[queue.front];
    printf("[THREAD %d] ==> consumed: %d\n", threadID, item);
    usleep(rand() % 10000 );
    queue.front = (queue.front + 1) % BUFFER_SIZE;
    queue.size -= 1;
}

void* producer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sem_wait(&empty); // Se não tiver espaça no buffer, espera
        sem_wait(&critical_region); //Se a região critica tiver sendo usada, espera.
        produce(thread_id); // Produz
        sem_post(&critical_region); // Libera a regiao critica
        sem_post(&full); // aumenta uma unidade no full
    }
    return NULL;
}

void* consumer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < ((PRODUCERS * BUFFER_SIZE) / CONSUMERS); i++) {
        sem_wait(&full);
        sem_wait(&critical_region);
        consume(thread_id);
        sem_post(&critical_region);
        sem_post(&empty);
    }
    return NULL;
}

int main() {
    queue.front = 0;
    queue.back = 0;
    queue.size = 0;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&critical_region, 0, 1);

    pthread_t threads[CONSUMERS + PRODUCERS];
    int thread_ids[CONSUMERS + PRODUCERS];


    for (int i = 0; i < PRODUCERS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, producer, &thread_ids[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        thread_ids[PRODUCERS + i] = PRODUCERS + i;
        pthread_create(&threads[PRODUCERS + i], NULL, consumer, &thread_ids[PRODUCERS + i]);
    }

    for (int i = 0; i < CONSUMERS + PRODUCERS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&critical_region);

    return 0;
}
