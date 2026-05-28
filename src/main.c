// main.c - Inicio principal del minikernel
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "pcb.h"
#include "queue.h"
#include "metrics.h"
#include "sync.h"

pthread_mutex_t metrics_mutex = PTHREAD_MUTEX_INITIALIZER;

void *cpu_run(void *arg);

// Variables globales compartidas entre todos los hilos
ready_queue_t ready_queue;
metrics_t metrics;
int system_running = 1;     // sistema activo
int current_time = 0;       // tiempo actual del sistema
int pid_counter = 1;        // contador de PIDs

// Hilo generador de procesos
void *process_generator(void *arg) {
    (void)arg;
    printf("Generando procesos...\n");

    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        int espera = (rand() % 3) + 1;
        sleep(espera);
        current_time += espera;

        // Crea y asigna todas las variables a un proceso
        pcb_t proceso;
        proceso.pid            = pid_counter++;
        proceso.burst_time     = (rand() % 8) + 1;  // entre 1 y 8
        proceso.remaining_time = proceso.burst_time;
        proceso.priority       = (rand() % 5) + 1;  // entre 1 y 5
        proceso.arrival_time   = current_time;
        proceso.state          = READY;

        printf("[GEN] Proceso %d agregado | burst=%d | llegada=%d\n",
               proceso.pid, proceso.burst_time, proceso.arrival_time);

        enqueue(&ready_queue, proceso); // Inserta el proceso en la cola
    }

    printf("[GEN] Todos los procesos han sido creados\n\n");
    return NULL;
}

int main() {
    srand(42);  // Semilla para números aleatorios

    // Inicializa la cola y las métricas
    queue_init(&ready_queue);
    metrics_init(&metrics);

    // Crea los hilos de las CPUs
    pthread_t cpu_threads[N_CPUS];
    int cpu_ids[N_CPUS];

    printf("Iniciando MiniKernel con %d CPUs...\n", N_CPUS);

    // Crea CPUs
    for (int i = 0; i < N_CPUS; i++) {
        cpu_ids[i] = i;
        if (pthread_create(&cpu_threads[i], NULL, cpu_run, &cpu_ids[i]) != 0) {
            printf("Error creando CPU %d\n", i);
        }
    }

    // Hilo que genera procesos
    pthread_t generator_thread;
    if (pthread_create(&generator_thread, NULL, process_generator, NULL) != 0) {
        printf("No se pudo crear el generador de procesos\n");
        return 1;
    }

    // Espera que termine el generador
    pthread_join(generator_thread, NULL);

    
    // Espera hasta que todos los procesos terminen
	while (1) {

    	pthread_mutex_lock(&metrics_mutex);
        int count = metrics.count;
     	pthread_mutex_unlock(&metrics_mutex);

        if (count == TOTAL_PROCESSES)
        break;

    sleep(1);
    }


    system_running = 0;

    // Despierta CPUs bloqueadas en dequeue
    for (int i = 0; i < N_CPUS; i++) {
        pthread_cond_broadcast(&ready_queue.not_empty);
    }

    // Espera a que todas las CPUs terminen
    for (int i = 0; i < N_CPUS; i++) {
        pthread_join(cpu_threads[i], NULL);
    } 

    metrics_print(&metrics);

    queue_destroy(&ready_queue);
 

    printf("\nSistema finalizado\n");
    return 0;
}
