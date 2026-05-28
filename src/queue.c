// queue.c - Implementación de procesos en cola

#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "sync.h"

extern int system_running;

// Inicializa la cola vacía
void queue_init(ready_queue_t *q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;

    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

// Inserta un proceso al final de la cola
void enqueue(ready_queue_t *q, pcb_t process) {
    pthread_mutex_lock(&q->mutex);

    // Espera si la cola está llena
    while (queue_is_full(q)) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    // Inserta el proceso
    q->processes[q->rear] = process;
    q->rear = (q->rear + 1) % MAX_PROCESSES;
    q->size++;

    // Avisa que hay procesos disponibles
    pthread_cond_signal(&q->not_empty);

    pthread_mutex_unlock(&q->mutex);
}

// Atiende el primer proceso de la cola
pcb_t dequeue(ready_queue_t *q) {
    pthread_mutex_lock(&q->mutex);

    // Espera mientras la cola esté vacía
    // y el sistema siga funcionando
    while (queue_is_empty(q) && system_running) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }

    // Si el sistema terminó y ya no hay procesos
    if (!system_running && queue_is_empty(q)) {
        pcb_t empty = {0};

        pthread_mutex_unlock(&q->mutex);
        return empty;
    }

    // Obtiene el siguiente proceso
    pcb_t process = q->processes[q->front];

    q->front = (q->front + 1) % MAX_PROCESSES;
    q->size--;

    // Avisa que hay espacio disponible
    pthread_cond_signal(&q->not_full);

    pthread_mutex_unlock(&q->mutex);

    return process;
}

// Verifica si la cola está vacía
bool queue_is_empty(ready_queue_t *q) {
    return q->size == 0;
}

// Verifica si la cola está llena
bool queue_is_full(ready_queue_t *q) {
    return q->size == MAX_PROCESSES;
}

// Libera recursos de sincronización
void queue_destroy(ready_queue_t *q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}
