#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

#define BUFFER_SIZE 5
#define CONSUMERS 6
#define PRODUCERS 15

typedef struct Queue {
    int buffer[BUFFER_SIZE];
    int front;
    int back;
    int size;
} Queue;

Queue queue;
sem_t empty;
sem_t full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int produce(int threadID) {
    int item = rand() % 10000;
    queue.buffer[queue.back] = item;
    printf("%s[THREAD %d] ==> produced: %d\n%s", KGRN, threadID, item, KWHT);
    queue.back = (queue.back + 1) % BUFFER_SIZE;
    queue.size += 1;
    return item;
}

void consume(int threadID) {

    if(queue.size <= 0 ) return;

    int item = queue.buffer[queue.front];
    printf("[THREAD %d] ==> consumed: %d\n", threadID, item);
    queue.front = (queue.front + 1) % BUFFER_SIZE;
    queue.size -= 1;
}

void* producer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex); // Depois ver como remover o mutex e usar apenas o semafaro
        produce(thread_id);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void* id) {
    int thread_id = *((int *)id);
    for (int i = 0; i < ((PRODUCERS * BUFFER_SIZE) / CONSUMERS); i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        consume(thread_id);
        pthread_mutex_unlock(&mutex);
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
    pthread_mutex_destroy(&mutex);

    return 0;
}
