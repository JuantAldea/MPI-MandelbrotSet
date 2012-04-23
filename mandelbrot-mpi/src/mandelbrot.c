/*
 * mandelbrot.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "ppm.h"
#include "algorithms.h"

void sparse_rows_version(int argc, char *argv[]) {
    window win;
    win.pixels_height = 2048;
    win.pixels_width = 2048;
    win.x_len = 4.0;
    win.y_len = 4.0;
    win.x_start = -2.0;
    win.y_start = -2.0;
    int max_iters = 100;

    int com_rank, com_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    unsigned char *chunk = NULL;
    unsigned char *buffer = NULL;

    printf("Rank: %d, %d %d\n", com_rank, com_rank * win.pixels_height / com_size, win.pixels_height / com_size);

    //chunk = mandelbrot(com_rank * pixels_height / com_size, pixels_height / com_size);
    chunk = sparse_rows_mandelbrot(win, com_rank, win.pixels_height / com_size, com_size, max_iters);

    int buffer_size = win.pixels_width * win.pixels_height / com_size;

    if (com_rank == 0) {
        buffer = (unsigned char *) malloc(sizeof(unsigned char) * win.pixels_height * win.pixels_width);
    }

    MPI_Gather(chunk, buffer_size, MPI_UNSIGNED_CHAR, buffer, buffer_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    char path[20];
    sprintf(path, "%d.pgm", com_rank);

    write_pgm(path, win.pixels_height, win.pixels_width/com_size, 255, chunk);

    free(chunk);

    //Output
    if (com_rank == 0) {
        unsigned char *ordered_im = sparse_rows_image_reconstruction(com_size, win.pixels_height, win.pixels_width, buffer);
        write_pgm("mandelbrot.pgm", win.pixels_height, win.pixels_width, 255, ordered_im);
        free(ordered_im);
        free(buffer);
    }

    MPI_Finalize();
}
int main(int argc, char* argv[]) {

    sparse_rows_version(argc, argv);

    return EXIT_SUCCESS;
}

