
CFILES := $(wildcard *.c)
TARGET := mandelbrot
CFLAGS := -Wall -Wextra -pedantic -O3 -std=c99

MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)

########################

# -MMD generates dependencies while compiling
CFLAGS += -MMD
CC := gcc

OBJFILES := $(CFILES:.c=.o)
DEPFILES := $(CFILES:.c=.d)

$(TARGET) : $(OBJFILES)
	$(LINK.o) $(CFLAGS) $(MPI_COMPILE_FLAGS) $(MPI_LINK_FLAGS) -o $@ $^

clean :
	rm -f $(TARGET) $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)
