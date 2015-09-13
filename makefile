CC=g++
CFLAGS=-O3 -ffast-math -fwrapv -std=c++11 
CP=-std=c++11
IS=100
all:
	$(CC) $(CP) scriptRunner.cpp -o ScriptRunner
	make MParallelOptimized
	
MInit:
	$(CC) $(CFLAGS) main.cpp -fopenmp -DSIZE=1000 -o MatrixMultiplication
	./MatrixMultiplication

MTest:
	./MatrixMultiplication

MSerial:
	for number in 100 200 300 400 500 600 700 800 900 1000; \
	do \
	$(CC) $(CFLAGS) serial.cpp -fopenmp -DSIZE=$$number -o Serial; \
	./ScriptRunner Serial $(IS) >> "serial/serial"$$number".txt"; \
	done

MParallel:
	for number in 100 200 300 400 500 600 700 800 900 1000; \
	do \
	$(CC) $(CFLAGS) parallel.cpp -fopenmp -DSIZE=$$number -o Parallel; \
	./ScriptRunner Parallel $(IS) >> "parallel/parallel"$$number".txt"; \
	done

MParallelOptimized:
	for number in 100 200 300 400 500 600 700 800 900 1000; \
	do \
	$(CC) $(CFLAGS) parallel_optimized.cpp -fopenmp -DSIZE=$$number -o ParallelOptimized; \
	./ScriptRunner ParallelOptimized $(IS) >> "parallelOptimized/parallelOptimized"$$number".txt"; \
	done
