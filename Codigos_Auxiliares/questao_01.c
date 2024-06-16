// Author: Chat GPT
// Código de exemplo

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Saldo inicial do banco
int saldo = 1000;
pthread_mutex_t saldo_mutex = PTHREAD_MUTEX_INITIALIZER;

// Função para leitores (consultar saldo)
void *leitor(void *id) {
    int thread_id = *((int *)id);
    usleep(rand() % 1000);  // Simula o tempo de leitura
    printf("Leitor %d lê o saldo: %d\n", thread_id, saldo);
    return NULL;
}

// Função para escritores (realizar saque ou depósito)
void *escritor(void *id) {
    int thread_id = *((int *)id);
    usleep(rand() % 1000);  // Simula o tempo de escrita

    int operacao = rand() % 2;
    int valor = (rand() % 91) + 10;  // Valor entre 10 e 100

    if (operacao == 0) {
        // Depósito
        saldo += valor;
        printf("Escritor %d deposita %d. Saldo atualizado: %d\n", thread_id, valor, saldo);
    } else {
        // Saque
        if (saldo >= valor) {
            saldo -= valor;
            printf("Escritor %d saca %d. Saldo atualizado: %d\n", thread_id, valor, saldo);
        } else {
            printf("Escritor %d tenta sacar %d, mas saldo insuficiente. Saldo atual: %d\n", thread_id, valor, saldo);
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[8];
    int thread_ids[8];
    int i;

    // Inicializando IDs das threads
    for (i = 0; i < 8; i++) {
        thread_ids[i] = i;
    }

    // Criando threads leitoras
    for (i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, leitor, (void *)&thread_ids[i]);
    }

    // Criando threads escritoras
    for (i = 5; i < 8; i++) {
        pthread_create(&threads[i], NULL, escritor, (void *)&thread_ids[i]);
    }

    // Aguardando a conclusão de todas as threads
    for (i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Operações concluídas. Saldo final: %d\n", saldo);

    return 0;
}
