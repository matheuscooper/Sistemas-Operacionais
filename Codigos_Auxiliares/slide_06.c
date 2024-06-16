#include <stdio.h>
#include <assert.h>
#include <pthread.h>

void* minhaThread(void* arg){

    printf("%s\n", (char*) arg);
    return NULL;

}

int main(){
    
    pthread_t p1, p2;

    int rc;

    printf("main:begin\n");

    rc = pthread_create(&p1, NULL, minhaThread, "A"); assert(rc == 0);
    rc = pthread_create(&p2, NULL, minhaThread, "B"); assert(rc == 0);

    rc = pthread_join(p1, NULL); assert(rc == 0);
    rc = pthread_join(p2, NULL); assert(rc == 0);

    printf("main:end\n");
}