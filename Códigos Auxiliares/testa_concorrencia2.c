#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#define NUM_THREADS	2

int saldo; // variável global compartilhada

void *deposito(void *t)
{  long tid;
   int s;
   tid = (long)t;
   printf("Thread %ld de deposito lendo o saldo…\n",tid);
   sleep(2); // simula o tempo de leitura no arquivo para pegar o saldo
   s = saldo;
   printf("Thread %ld de deposito leu o saldo = %d\n",tid, s);
   s = s + 10;
   printf("Thread %ld de saque escrevendo o saldo = %d\n",tid, s);
   sleep(3); // simula o tempo de escrita no arquivo para atualizar o saldo
   saldo = s;
   printf("Thread %ld - valor do saldo = %d\n", tid, saldo);
   printf("Thread %ld done.\n",tid);
}

void *saque(void *t)
{  long tid;
   int s;
   tid = (long)t;
   printf("Thread %ld de saque lendo o saldo…\n",tid);
   sleep(2); // simula o tempo de leitura no arquivo para pegar o saldo
   s = saldo;
   printf("Thread %ld de saque leu o saldo = %d\n",tid, s);
   s = s - 15;
   printf("Thread %ld de saque escrevendo o saldo = %d\n",tid, s);
   sleep(3); // simula o tempo de escrita no arquivo para atualizar o saldo
   saldo = s;
   printf("Thread %ld - valor do saldo = %d\n", tid, saldo);
   printf("Thread %ld done.\n",tid);
}


int main(int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc; 
   long t;

   /* inicializa saldo */
   saldo=100;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   for(t=0;t<NUM_THREADS;t++) {
      printf("Main: creating thread %ld\n", t);
      if (t % 2 == 0)
         rc = pthread_create(&thread[t], &attr, deposito, (void *)t); 
      else
         rc = pthread_create(&thread[t], &attr, saque, (void *)t); 
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* We're done with the attribute object, so we can destroy it */
   pthread_attr_destroy(&attr);


   /* The main thread is done, so we need to call pthread_exit explicitly to permit the working threads to continue even after main completes.
   */

   sleep(8); // main dorme bastante para acordar após as thread terem terminado e pegar o saldo final 

// Espera as threads terminarem para mostrar como ficou o saldo no final na main
//   pthread_join(thread[0], NULL);
//   pthread_join(thread[1], NULL);

   printf("Valor final do saldo = %d\n", saldo);
   printf("Main: programa terminou\n");
   pthread_exit(NULL);
   return(0);
}