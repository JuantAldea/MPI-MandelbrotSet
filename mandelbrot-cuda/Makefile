CC = nvcc
CFLAGS = -g -m64

all: mandelbrot

mandelbrot: main.o ppm.o
	$(CC) $(CFLAGS) -o mandelbrot main.o ppm.o -L/usr/local/cuda/lib -lcudart

main.o: main.cu
	$(CC) $(CFLAGS) -c main.cu

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c ppm.c

clean:
	rm -f mandelbrot *.o