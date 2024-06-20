#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

#define BUFFER_SIZE 5
#define CONSUMERS 5
#define PRODUCERS 5

typedef struct Queue {
    int buffer[BUFFER_SIZE];
    int front;
    int back;
    int size;
} Queue;

Queue queue;

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
    usleep(rand() % 1000 );
    queue.front = (queue.front + 1) % BUFFER_SIZE;
    queue.size -= 1;
}

void* producer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < BUFFER_SIZE; i++) {
        produce(thread_id); // Produz
    }
    return NULL;
}

void* consumer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < ((PRODUCERS * BUFFER_SIZE) / CONSUMERS); i++) {
        consume(thread_id);
    }
    return NULL;
}

int main() {
    queue.front = 0;
    queue.back = 0;
    queue.size = 0;


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

   
    return 0;
}
