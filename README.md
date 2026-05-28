# MiniKernel

Simulación de un mini sistema operativo implementado en C, utilizando hilos POSIX (pthread) para representar CPUs y el algoritmo Round Robin para ejecutar procesos.

## Descripción

El sistema MiniKernel simula el funcionamiento básico de un kernel al administrar procesos y CPUs concurrentes.

Incluye:

* **Generador de procesos**: crea procesos simulados mediante la estructura PCB
* **Cola de procesos listos**: estructura compartida protegida con mutex y variables condicionales
* **CPUs simuladas**: hilos POSIX que ejecutan procesos usando Round Robin
* **Métricas**: mide el rendimiento del scheduler

## Estructura del proyecto

```text
MiniKernel/
├── include/
│   ├── pcb.h
│   ├── queue.h
│   ├── metrics.h
│   └── sync.h
│
├── src/
│   ├── main.c
│   ├── queue.c
│   ├── cpu.c
│   └── metrics.c
│
├── tests/
│   └── test_main.c
│
├── Makefile
└── README.md
```

## Requisitos

* GCC
* Make
* Linux o WSL2

## Compilar y ejecutar

```bash
make
./minikernel
```

## Correr tests

```bash
make test
```

## Ejemplo de salida

```text
[CPU 1] Ejecutando Proceso 2 | restante: 5
[CPU 1] Proceso 2 finalizado
```

## Métricas generadas

* Turnaround promedio
* Tiempo de espera promedio
* Tiempo de respuesta promedio
* Throughput

## Autor

* Erik Yupa
* Escuela Politécnica Nacional
