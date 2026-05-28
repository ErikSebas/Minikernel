
// pcb.h - Define la estructura PCB que representa cada proceso en el sistema

#ifndef PCB_H        // Evita incluir el archivo varias veces
#define PCB_H

typedef enum {  // Diferentes estados en los que puede estar el proceso
    NEW,
    READY,
    RUNNING,
    TERMINATED
} process_state_t; 

// Campos que describen el estado y variables de cada proceso
typedef struct {
    int pid;                      // Identificador unico del proceso
    int burst_time;               // Tiempo total que necesita para ejecutar
    int remaining_time;		  // Tiempo restante a ejecutar	
    int arrival_time;		  // Tiempo de llegada
    int priority;                 // Prioridad del proceso   
    int state;                    // Estado actual del proceso
} pcb_t;

#endif
