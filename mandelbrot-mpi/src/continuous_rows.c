/*
 * continuous_rows.c
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

#include "ppm.h"
#include "mandelbrot_iteration.h"

#include "continuous_rows.h"


uchar *continuous_rows_mandelbrot(window win, int first_row, int n_rows, int max_iter) {

    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float cr, ci;
    int i, j;

    uchar *image = (uchar *) malloc(sizeof(uchar) * n_rows * win.pixels_width);

    for (j = first_row; j < first_row + n_rows; j++) {
        for (i = 0; i < win.pixels_width; i++) {
            ci = (float) win.y_start + (float) j * dy;
            cr = (float) win.x_start + (float) i * dx;
            image[(j - first_row) * win.pixels_width + i] = (mandelbrot_iteration(cr, ci, max_iter) == max_iter) ? 255 : 0;
        }
    }
    return image;
}

uchar *mandelbrot_x_axis(window win, int first_pixel, int n_pixels, int max_iter){
    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float cr, ci;
    int j;

    uchar *image = (uchar *) malloc(sizeof(uchar) * n_pixels);

    for (j = first_pixel; j < first_pixel + n_pixels; j++) {
        ci = (float) win.y_start + (float) (win.pixels_height/2) * dy;
        cr = (float) win.x_start + (float) j * dx;
        image[(j - first_pixel)] = (mandelbrot_iteration(cr, ci, max_iter) == max_iter) ? 255 : 0;
    }
    return image;
}

void continuous_rows_version(int argc, char *argv[]) {


    MPI_Init(&argc, &argv);
    window win;
    int max_iters = atoi(argv[2]);
    win.pixels_height = atoi(argv[1]);
    win.pixels_width = win.pixels_height;
    win.x_start = -2;
    win.x_len = 0.8 + 2;
    win.y_start = -1.5;
    win.y_len = 1.5 + 1.5;

    int com_rank, com_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    uchar *chunk = NULL;
    uchar *buffer = NULL;

    int first_row = com_rank * win.pixels_height / com_size / 2;
    int n_rows    = win.pixels_height / com_size / 2;
    printf("Rank: %d, %d %d\n", com_rank, first_row, n_rows);

    chunk = continuous_rows_mandelbrot(win, first_row, n_rows, max_iters);

    uchar *axis_chunk = NULL;
    axis_chunk = mandelbrot_x_axis(win, com_rank * win.pixels_width/com_size, win.pixels_width/com_size, max_iters);

    int buffer_size = win.pixels_width * n_rows;

    if (com_rank == 0) {
        buffer = (uchar *) malloc(sizeof(uchar) * (win.pixels_height + 1) * win.pixels_width);
    }

    MPI_Gather(chunk, buffer_size, MPI_UNSIGNED_CHAR, buffer, buffer_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gather(axis_chunk, win.pixels_width/com_size, MPI_UNSIGNED_CHAR, (buffer + win.pixels_height / 2 * win.pixels_width), win.pixels_width/com_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    char path[20];
    sprintf(path, "%d.pgm", com_rank);

    write_pgm(path, n_rows, win.pixels_width, 255, chunk);

    free(chunk);

    //Output
    if (com_rank == 0) {
        int i;
        for (i = 1; i < win.pixels_height/2; i++){
            memcpy(buffer  + win.pixels_width * (win.pixels_height - i),
                    buffer + win.pixels_width * i,
                    win.pixels_width);
        }

        write_pgm("mandelbrot.pgm", win.pixels_height, win.pixels_width, 255, buffer);
        free(buffer);
    }

    MPI_Finalize();
}

