#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t empty, fill;

pthread_mutex_t mutex;

#define MAX 150

int count = 0;

void* producer(void* arg){

    for(int i=0; i < MAX; i++){
        
        pthread_mutex_lock(&mutex);

        while(count == MAX){
            pthread_cond_wait(&empty,&mutex);
        }
        // Produz

        pthread_cond_signal(&fill); // Wake up all threads that are waiting for this condition.
        pthread_mutex_unlock(&mutex);
    }
}



int main(){

}