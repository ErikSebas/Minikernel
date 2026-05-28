// test_main.c - Tests de integración del MiniKernel

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../include/queue.h"
#include "../include/metrics.h"
#include "../include/sync.h"
#include "../include/pcb.h"

// Variable global
int system_running = 1;

// La cola inicia vacía
void test_queue_starts_empty() {
    ready_queue_t q;
    queue_init(&q);
    if (!queue_is_empty(&q)) {
        printf("✗ Error: la cola debería iniciar vacía\n");
        exit(1);
    }
    printf("✓ Inicializa la cola\n");
    queue_destroy(&q);
}

// Insertar y sacar un proceso de la cola
void test_enqueue_dequeue() {
    ready_queue_t q;
    queue_init(&q);

    pcb_t p;
    p.pid            = 1;
    p.burst_time     = 5;
    p.remaining_time = 5;
    p.arrival_time   = 0;
    p.state          = READY;

    enqueue(&q, p);

    if (queue_is_empty(&q)) {
        printf("✗ Error: la cola debería tener un proceso\n");
        exit(1);
    }

    pcb_t result = dequeue(&q);

    if (result.pid != 1 || result.burst_time != 5) {
        printf("✗ Error: el proceso sacado no coincide\n");
        exit(1);
    }

    assert(queue_is_empty(&q) == true);
    printf("✓ Test: enqueue y dequeue funcionan correctamente\n");
    queue_destroy(&q);
}

// Metricas inicializadas
void test_metrics_start_zero() {
    metrics_t m;
    metrics_init(&m);

    if (m.count != 0 || m.avg_turnaround != 0.0 || m.avg_waiting != 0.0) {
        printf("✗ Error: las métricas deberían iniciar en cero\n");
        exit(1);
    }

    printf("✓ Test: las métricas inician en cero\n");
}

// Registrar Metricas
void test_metrics_record() {
    metrics_t m;
    metrics_init(&m);

    process_metrics_t pm;
    pm.pid             = 1;
    pm.arrival_time    = 0;
    pm.burst_time      = 4;
    pm.completion_time = 6;
    pm.turnaround_time = 6;
    pm.waiting_time    = 2;
    pm.response_time   = 1;

    metrics_record(&m, pm);

    if (m.count != 1 || m.data[0].pid != 1) {
        printf("✗ Error: la métrica no se registró correctamente\n");
        exit(1);
    }

    printf("✓ Test: métrica registrada correctamente\n");
}

int main() {
    printf("\n----- Tests del Minikernel -----\n\n");

    test_queue_starts_empty();
    test_enqueue_dequeue();
    test_metrics_start_zero();
    test_metrics_record();

    printf("\n✓ Todos los tests pasaron correctamente\n");
    printf("--------------------------------\n\n");
    return 0;
}
