#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h> 

#define NUM_THREADS 4

typedef struct Registro
{
    int nota;
} Registro;

Registro registro; // é a mesma variável global do codigo dele, mas para o registro das notas 

pthread_mutex_t mutexLeitor; 
pthread_cond_t condLeitores;
pthread_cond_t condEscritores; // variaveis para as condições tanto para leitores como escritores e sincronizar as threads

// tanto número de leitores ativos como escritores ativos, então preciso saber quando há letoires e escritores ativos de modo que os leitores acessem os dados
int leitoresAtivos = 0;
int escritoresAtivos = 0;
// isso aqui é para dar prioridade para os escritores em relação aos leitores
int threadEscritores = 0; /// acho que não preciso disso aqui

void *leitor(void *t){
    long tid = (long)t;
    pthread_mutex_lock(&mutexLeitor); 
    while( escritoresAtivos > 0 ){
        pthread_cond_wait(&condLeitores, &mutexLeitor); // bloqueia o acesso por outro fio ao mutex, e em seguida, a variável de condição.
    }
    leitoresAtivos++;
    pthread_mutex_unlock(&mutexLeitor);

    printf("Thread %ld de leitura lenda a nota\n", tid);
    sleep(2); 
    int notaLida = registro.nota;
    printf("Thread %ld de leitura leu a nota %d\n", tid, notaLida);
    printf("Thread %ld de leitura acabou :)\n", tid);

    // se ele leu e não tem mais leitores significa que os escritores podem ter acesso 
    pthread_mutex_lock(&mutexLeitor);
    leitoresAtivos--;
    // se não há leitores ativos, então eu ativo a condição dos escritores por isso a poha do contador
    if( leitoresAtivos == 0){
        pthread_cond_signal(&condEscritores); // desbloquea uma thread bloqueada em uma condição
    }
    pthread_mutex_unlock(&mutexLeitor);
    pthread_exit(NULL); 
}

void *escritor(void*t){
    long tid = (long)t;
    pthread_mutex_lock(&mutexLeitor); // bloqueio os leitores 
    ///threadEscritores++; // aumenta o numero de threads esperando para escrever, como um leitor acabou de ler, então eu preciso verificar se tem escritores esperando, aí eu sinalizo para os escritores
    while( leitoresAtivos > 0 ){
        pthread_cond_wait(&condEscritores, &mutexLeitor); 
    }
    escritoresAtivos++;
    pthread_mutex_unlock(&mutexLeitor);

    printf("thread %ld de escrita lenda a nota\n", tid);
    sleep(2);
    int notaAtual = registro.nota;
    printf("thread %ld de escrita leu a nota %d\n", tid, notaAtual);
    int notaAtualizada = notaAtual + 6;
    printf("thread %ld de escrita leu a nota atualizada %d\n", tid, notaAtualizada);
    usleep(3);
    registro.nota = notaAtualizada; // aqui eu atualizo a nota no registro
    printf("thread %ld de escrita atualizou a nota no registro para %d\n", tid, registro.nota);
    printf("Thread %ld de escrita acabou\n", tid);

    // bloqueio os leitores novamente e decremento o numero de escritores ativos para ativar a condição dos leitores a fim de que eles leiam agora - 
    /// mas preciso saber se nao tem escritores esperando, se houver eu bloqueiou a condição dos escritores 
    pthread_mutex_lock(&mutexLeitor);
    escritoresAtivos--;
    if( escritoresAtivos > 0){
        pthread_cond_signal(&condEscritores);
    }
    else{
        pthread_cond_broadcast(&condLeitores); //desbloqueia todos os leitores que estavam bloqueados
    }
    pthread_mutex_unlock(&mutexLeitor);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    int rc;
    long t;

    pthread_mutex_t mutexLeitor; 
    pthread_cond_t condLeitores;
    pthread_cond_t condEscritores; 
    // inicia os registro
    registro.nota = 50;

    // inicializa mutex e variáveis de condição
    pthread_mutex_init(&mutexLeitor, NULL);
    pthread_cond_init(&condLeitores, NULL);
    pthread_cond_init(&condEscritores, NULL);

    int readThread = 0;
    int writerThreads = 0;
    int maxThreadsPerOperation = NUM_THREADS / 2;

    for(t = 0; t < NUM_THREADS; t++) {
        if (((rand() % 2 == 0 && readThread < maxThreadsPerOperation)) || writerThreads >= maxThreadsPerOperation){
            rc = pthread_create(&thread[t], NULL, leitor, (void *)t);
            printf("Main: criando a thread leitora %ld\n", t);
            readThread++;
        }  
        else{
            rc = pthread_create(&thread[t], NULL, escritor, (void *)t);
            printf("Main: criando a thread escritora %ld\n", t);
            writerThreads++;
            
        }  
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }


    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(thread[t], NULL);
    }

    // destrói mutex e variáveis de condição
    pthread_mutex_destroy(&mutexLeitor);
    pthread_cond_destroy(&condLeitores);
    pthread_cond_destroy(&condEscritores);

    printf("Nota final no registro = %d\n", registro.nota);
    printf("Main: programa terminou\n");
    pthread_exit(NULL);
    return 0;
}