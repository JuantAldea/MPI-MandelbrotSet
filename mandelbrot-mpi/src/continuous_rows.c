/*
 * continuous_rows.c
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "ppm.h"
#include "continuous_rows.h"
#include "math_optimizations.h"

unsigned char *continuous_rows_mandelbrot(window win, int first_row, int n_rows, int max_iter) {

    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float zr, zi, cr, ci, zrs, zis;
    int i, j, color;
    i = j = -1;
    unsigned char *image = (unsigned char *) malloc(sizeof(unsigned char) * n_rows * win.pixels_width);

    for (j = first_row; j < first_row + n_rows; j++) {
        for (i = 0; i < win.pixels_width; i++) {
            zi = ci = (float) win.y_start + (float) j * dy;
            zr = cr = (float) win.x_start + (float) i * dx;
            zrs = zis = (float) 0;
            color = 0;

            if (test_point_in_cardiod_or_2ndbud(zr, zi)) {
                color = max_iter;
            } else {
                while (zrs + zis < (float) 4 && color < max_iter) {
                    zrs = zr * zr;
                    zis = zi * zi;
                    zi = (float) 2 * zr * zi + ci;
                    zr = zrs - zis + cr;
                    color++;
                }
            }

            image[(j - first_row) * win.pixels_width + i] = (color == max_iter) ? 255 : 0;
        }
    }
    return image;
}


void continuous_rows_version(int argc, char *argv[]) {
    window win;
    win.pixels_height = 2048;
    win.pixels_width = 2048;
    win.x_len = 0.8 + 2;
    win.y_len = 1.5 + 1.5;
    win.x_start = -2;
    win.y_start = -1.5;
    int max_iters = 100;

    int com_rank, com_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    unsigned char *chunk = NULL;
    unsigned char *buffer = NULL;

    int first_row = com_rank * win.pixels_height / com_size;
    int n_rows    = win.pixels_height / com_size;
    printf("Rank: %d, %d %d\n", com_rank, first_row, n_rows);

    chunk = continuous_rows_mandelbrot(win, first_row/2, n_rows/2, max_iters);

    int buffer_size = win.pixels_width * n_rows/2;

    if (com_rank == 0) {
        buffer = (unsigned char *) malloc(sizeof(unsigned char) * win.pixels_height * win.pixels_width);
    }

    MPI_Gather(chunk, buffer_size, MPI_UNSIGNED_CHAR, buffer, buffer_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    char path[20];
    sprintf(path, "%d.pgm", com_rank);

    write_pgm(path, n_rows/2, win.pixels_width, 255, chunk);

    free(chunk);

    //Output
    if (com_rank == 0) {
        write_pgm("mandelbrot.pgm", win.pixels_height, win.pixels_width, 255, buffer);
        free(buffer);
    }

    MPI_Finalize();
}

