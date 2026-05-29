# Makefile - Compilación del MiniKernel

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -lpthread
SRC = src/main.c src/queue.c src/cpu.c src/metrics.c
TARGET = MiniKernel
TEST = tests/test_main.c

# Compila el proyecto completo
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Compila y corre los tests
test:
	$(CC) $(CFLAGS) -o test_run $(TEST) src/queue.c src/metrics.c
	./test_run

# Limpia los archivos compilados
clean:
	rm -f $(TARGET) test_run

.PHONY: all test clean
