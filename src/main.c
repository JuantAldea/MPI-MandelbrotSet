/*
 * mandelbrot.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "algorithms.h"

void ayuda(char *nombre)
{
    printf("Static task asignment 0\n");
    printf("\t%s 0 <canvas_size> <iterations>\n", nombre);
    printf("Master-slave\n");
    printf("\t%s 1 <canvas_size> <iterations> <task_size>\n", nombre);
    printf("== Canvas size must be a power of two == \n");
}

//http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
int is_power_of_two(unsigned int x)
{
    return ((x != 0) && ((x & (~x + 1)) == x));
}

int main(int argc, char *argv[])
{
    //system("rm -f *pgm");
    //system("rm -f *ppm");
    int algoritmo = -1;
    int error = 0;
    int tam = -1;
    int iteraciones = -1;
    int tam_tarea = -1;

    if (argc > 1) {
        algoritmo = atoi(argv[1]);
        if (algoritmo < 0 || algoritmo > 1) {
            error = 1;
        }
    } else {
        error = 1;
    }

    if (argc > 2) {
        tam = atoi(argv[2]);
        if (tam < 1 || !is_power_of_two(tam)) {
            error = 1;
        }
    } else {
        error = 1;
    }

    if (argc > 3) {
        iteraciones = atoi(argv[3]);
        if (iteraciones < 1) {
            error = 1;
        }
    } else {
        error = 1;
    }

    if (argc > 4) {
        tam_tarea = atoi(argv[4]);
        if (algoritmo == 1 && tam_tarea < 1) {
            error = 1;
        }
    }
    if (algoritmo == 1 && argc < 5) {
        error = 1;
    }

    if (error) {
        //only one node should print errors
        MPI_Init(&argc, &argv);
        int com_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
        if (com_rank == 0) {
            ayuda(argv[0]);
        }
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    switch (algoritmo) {
    case 0:
        continuous_rows_version(argc, argv);
        break;
    case 1:
        dynamic2(argc, argv);
        break;
    case 2:
        //sparse_rows_version(argc, argv);
        break;
    case 3:
        //dynamic_row_assignment(argc, argv);
        break;
    }
    return EXIT_SUCCESS;
}
