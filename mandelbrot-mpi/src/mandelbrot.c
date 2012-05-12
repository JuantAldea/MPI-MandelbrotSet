/*
 * mandelbrot.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "algorithms.h"

int main(int argc, char* argv[]) {

    //sparse_rows_version(argc, argv);

    continuous_rows_version(argc, argv);

    return EXIT_SUCCESS;
}

