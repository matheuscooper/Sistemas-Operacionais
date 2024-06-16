/*****************************************************************************
* FILE: detached.c
* DESCRIPTION:
*   This example demonstrates how to explicitly create a thread in a 
*   detached state. This might be done to conserve some system resources
*   if the thread never needs to join later. Compare with the join.c program
*   where the threads are created joinable.
* AUTHOR: 01/30/08 Blaise Barney, modified by JMB Cavalcanti
* LAST REVISED: 16/05/2019
* modificado para demonstrar o problema de concorrência. Variável x é 
* compartilhada entre as threads. O sleep dá um atraso suficiente para ocorrer
* o acesso concorrente à variável x e dá resultado errado. O valor final de x
* deveria ser 4000. Mas varia bastante. Execute várias vezes para verificar. 
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#define NUM_THREADS	4

int x;

void *faz_algo(void *t)
{
   long tid;
   int i;
   tid = (long)t;
//   sleep(2);
   printf("Thread %ld iniciando…x= %d\n",tid, x);
   for (i=1; i<=1000; i++)
   {   x = x + 1;
       if (x % 100 == 0)
       {   printf("%ld - %d\n", tid, x);
           printf("Thread %ld dormindo…\n",tid);
           sleep(1);
       }
   }
   printf("%ld - valor final de x = %d\n", tid, x);
   printf("Thread %ld done.\n",tid);
}

int main(int argc, char *argv[]) {
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc; 
   long t;

   /* inicializa x com 0 */
   x=0;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   for(t=0;t<NUM_THREADS;t++) {
      printf("Main: creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, faz_algo, (void *)t); 
      if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
      }
   }

   /* We're done with the attribute object, so we can destroy it */
   pthread_attr_destroy(&attr);


   /* The main thread is done, so we need to call pthread_exit explicitly to permit the     working threads to continue even after main completes.
   */

   printf("Valor de x na funcao main = %d\n", x);
   printf("Main: programa terminou.\n");
   pthread_exit(NULL);
   return 0;
   }