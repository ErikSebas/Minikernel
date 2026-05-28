// metrics.h - Métricas de rendimiento del scheduler

#ifndef METRICS_H
#define METRICS_H

#include "pcb.h" // Incluye el archivo pcb.h la estructura de cada proceso

#define MAX_PROCESSES 100 // Maximo de procesos en la cola

// Estructura que almacena las métricas de un proceso
// Nota: pid, arrival_time y burst_time son originales de pcb.h
typedef struct {
    int pid;                // Identificador unico del proceso
    int arrival_time;       // Tiempo de llegada
    int burst_time;         // Tiempo total que necesita para ejecutarse
    int completion_time;    // Tiempo en que termino de completarse
    int turnaround_time;    // Tiempo total dentro del sistema
    int waiting_time;       // Tiempo esperando en la cola
    int response_time;      // Tiempo hasta que se le dio CPU
} process_metrics_t;

// Estructura global de métricas del sistema
typedef struct {
    process_metrics_t data[MAX_PROCESSES]; // Métricas de cada proceso en un arreglo
    int count;                             // Cantidad de procesos registrados
    float avg_turnaround;                  // Promedio de turnaround(Cuanto tarda en completar todo)
    float avg_waiting;                     // Promedio de espera
    float avg_response;                    // Promedio de respuesta
    float throughput;                      // Procesos terminados por unidad de tiempo
} metrics_t;

// Funciones de métricas
void metrics_init(metrics_t *m);				// Inicializa las metricas: count, promedios y throughtput a cero
void metrics_record(metrics_t *m, process_metrics_t pm);	// Guarda las metricas de un proceso
void metrics_print(metrics_t *m);				// Muestra las metricas de cada proceso

#endif
