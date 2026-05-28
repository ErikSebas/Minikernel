// metrics.c - Cálculo e impresión de métricas de rendimiento del scheduler
#include <stdio.h>
#include "metrics.h"

// Inicializa las métricas en cero
void metrics_init(metrics_t *m) {
    m->count          = 0;
    m->avg_turnaround = 0.0;
    m->avg_waiting    = 0.0;
    m->avg_response   = 0.0;
    m->throughput     = 0.0;
}

// Registra las métricas de un proceso terminado
void metrics_record(metrics_t *m, process_metrics_t pm) {
    m->data[m->count] = pm;
    m->count++;
}

// Calcula e imprime todas las métricas al final
void metrics_print(metrics_t *m) {
    float total_turnaround = 0.0;
    float total_waiting    = 0.0;
    float total_response   = 0.0;

    printf("\n------------ Metricas del scheduler ------------\n\n");
    printf("Procesos ejecutados: %d\n\n", m->count);
    printf("%-5s %-10s %-10s %-12s %-12s %-10s\n",
           "PID", "Llegada", "Burst", "Turnaround", "Espera", "Respuesta");
    printf("-----------------------------------------------------\n\n");

    // Imprime métricas de cada proceso
    for (int i = 0; i < m->count; i++) {
        process_metrics_t pm = m->data[i];
        printf("%-5d %-10d %-10d %-12d %-12d %-10d\n\n",
               pm.pid, pm.arrival_time, pm.burst_time,
               pm.turnaround_time, pm.waiting_time, pm.response_time);
        total_turnaround += pm.turnaround_time;
        total_waiting    += pm.waiting_time;
        total_response   += pm.response_time;
    }

    // Promedios
    m->avg_turnaround = total_turnaround / m->count;
    m->avg_waiting    = total_waiting    / m->count;
    m->avg_response   = total_response   / m->count;
    m->throughput     = (float)m->count  / total_turnaround;

    printf("--- Promedios ---\n\n");
    printf("Turnaround promedio : %.2f\n", m->avg_turnaround);
    printf("Espera promedio     : %.2f\n", m->avg_waiting);
    printf("Respuesta promedio  : %.2f\n", m->avg_response);
    printf("Throughput          : %.2f procesos/unidad de tiempo\n", m->throughput);
    printf("\n---------------------------------------------\n");
}
