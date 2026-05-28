// cpu.c - Hilos de CPU para scheduling Round Robin

#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "sync.h"
#include "metrics.h"

extern ready_queue_t ready_queue;
extern metrics_t metrics;
extern int system_running;
extern int current_time;
extern pthread_mutex_t metrics_mutex;

// Ejecuta cada hilo CPU
void *cpu_run(void *arg) {

    int cpu_id = *(int *)arg;

    pthread_mutex_lock(&ready_queue.mutex);
    printf("[CPU %d] iniciada\n", cpu_id);
    pthread_mutex_unlock(&ready_queue.mutex);

    while (system_running) {

        pcb_t process = dequeue(&ready_queue);

        // Sale si el sistema terminó y no quedan procesos
        if (!system_running && process.pid == 0) {
            break;
        }

        process.state = RUNNING;

        pthread_mutex_lock(&ready_queue.mutex);

        printf("[CPU %d] Ejecutando Proceso %d | restante: %d\n",
               cpu_id,
               process.pid,
               process.remaining_time);

        pthread_mutex_unlock(&ready_queue.mutex);

        int t_respuesta = current_time - process.arrival_time;

        // Determina el tiempo de ejecución
        int t_ejecucion = (process.remaining_time < QUANTUM)
                            ? process.remaining_time
                            : QUANTUM;

        sleep(t_ejecucion);

        process.remaining_time -= t_ejecucion;

        // Proceso terminado
        if (process.remaining_time <= 0) {

            process.state = TERMINATED;

            pthread_mutex_lock(&ready_queue.mutex);

            printf("[CPU %d] Proceso %d finalizado\n\n",cpu_id, process.pid);

            pthread_mutex_unlock(&ready_queue.mutex);

            // Guarda métricas del proceso
            process_metrics_t pm = {
                .pid             = process.pid,
                .arrival_time    = process.arrival_time,
                .burst_time      = process.burst_time,
                .completion_time = current_time + t_ejecucion,
                .turnaround_time = (current_time + t_ejecucion) - process.arrival_time,
                .response_time   = t_respuesta
            };

            pm.waiting_time = pm.turnaround_time - pm.burst_time;

            pthread_mutex_lock(&metrics_mutex);

            metrics_record(&metrics, pm);

            pthread_mutex_unlock(&metrics_mutex);

        } else {

            process.state = READY;

            enqueue(&ready_queue, process);
        }
    }

    pthread_mutex_lock(&ready_queue.mutex);

    printf("[CPU %d] apagada\n", cpu_id);

    pthread_mutex_unlock(&ready_queue.mutex);

    return NULL;
}
