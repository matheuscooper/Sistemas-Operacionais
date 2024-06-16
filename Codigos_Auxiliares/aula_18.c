#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s;

void* child(void* args){

    printf("child\n");
    sem_post(&s);
    return NULL;

}

int main(int argc, char**argv){

    sem_init(&s, 0, 0);
    printf("Pai - COMEÇO\n");

    pthread_t c;

    pthread_create(&c, NULL, child, NULL);

    sem_wait(&s);
    printf("PAI- AcAbA\n");


}