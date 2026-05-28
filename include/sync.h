// sync.h - Sincronización global entre hilos 

#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

#define N_CPUS 4	// Procesos ejecutandose al mismo tiempo
#define QUANTUM 2   	// Tiempo maximo para proceso 

#define TOTAL_PROCESSES 10 // Total de procesos a crear

//Variable declarada en otro archivo, indica si el sistema sigue funcionando

extern pthread_mutex_t metrics_mutex;
extern int system_running;	
#endif
