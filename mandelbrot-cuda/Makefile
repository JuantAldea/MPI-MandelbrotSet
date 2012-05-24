CC = nvcc
CFLAGS = -g

all: mandelbrot

mandelbrot: main.cu ppm.o
	$(CC) $(CFLAGS) -o mandelbrot main.cu ppm.o

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c ppm.c

clean:
	rm -f mandelbrot *.o
