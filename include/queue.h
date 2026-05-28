
// queue.h - Procesos en Cola

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h> // Biblioteca para hilos  
#include <stdbool.h>   // Biblioteca para boolean
#include "pcb.h"    

#define MAX_PROCESSES 150 // Maximo de procesos en la cola

// Estructura de la cola de procesos listos
typedef struct {
    pcb_t processes[MAX_PROCESSES]; // Arreglo de todos los procesos en espera
    int front;                      // Primer proceso al inicio de la cola
    int rear;                       // Ultimo proceso al final de la cola
    int size;                       // Cantidad de procesos en la cola
				    
    // Control de concurrencia - evita condiciones de carrera entre hilos
    pthread_mutex_t mutex;          // Mutex para acceso exclusivo a la cola
    pthread_cond_t not_empty;       // Indica a los hilos que hay procesos disponibles
    pthread_cond_t not_full;        // Indica a los hilos que hay espacio 
} ready_queue_t;

// Funciones de la cola
	void queue_init(ready_queue_t *q);               // Inicializa la cola
	void enqueue(ready_queue_t *q, pcb_t process);   // Ingresa un proceso al final de la cola	
	pcb_t dequeue(ready_queue_t *q);                 // Atiende al primer proceso al inicio de la cola
	
	// Verifica si la cola esta llena o vacia con boolean
	bool queue_is_empty(ready_queue_t *q);			
	bool queue_is_full(ready_queue_t *q);		 
	void queue_destroy(ready_queue_t *q); 		 // Destruye y limpia la cola

#endif

