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

uchar *continuous_rows_mandelbrot(window win, int first_row, int n_rows,
                                  int max_iter)
{
    float dy = win.y_len / (float) win.pixels_height;
    float dx = win.x_len / (float) win.pixels_width;
    float cr, ci;
    int i, j;

    uchar *image =
        (uchar *) malloc(sizeof(uchar) *
                         (size_t) (n_rows * win.pixels_width));

    for (j = first_row; j < first_row + n_rows; j++) {
        for (i = 0; i < win.pixels_width; i++) {
            ci = (float) win.y_start + (float) j *dy;
            cr = (float) win.x_start + (float) i *dx;
            image[(j - first_row) * win.pixels_width + i] =
                (mandelbrot_iteration(cr, ci, max_iter) ==
                 max_iter) ? 255 : 0;
        }
    }
    return image;
}

uchar *mandelbrot_x_axis(window win, int first_pixel, int n_pixels,
                         int max_iter)
{
    float dy = win.y_len / (float) win.pixels_height;
    float dx = win.x_len / (float) win.pixels_width;
    float cr, ci;
    int j;

    uchar *image = (uchar *) malloc(sizeof(uchar) * (size_t) (n_pixels));

    for (j = first_pixel; j < first_pixel + n_pixels; j++) {
        ci = (float) win.y_start + (float) (win.pixels_height / 2) * dy;
        cr = (float) win.x_start + (float) j *dx;
        image[(j - first_pixel)] =
            (mandelbrot_iteration(cr, ci, max_iter) == max_iter) ? 255 : 0;
    }
    return image;
}

void continuous_rows_version(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    window win;
    int max_iters = atoi(argv[3]);
    win.pixels_height = atoi(argv[2]);
    win.pixels_width = win.pixels_height;
    win.x_start = -2;
    win.x_len = 0.8f + 2.0f;
    win.y_start = -1.5;
    win.y_len = 1.5 + 1.5;

    int com_rank, com_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    int first_row = com_rank * win.pixels_height / com_size / 2;
    int n_rows = win.pixels_height / com_size / 2;
    if (win.pixels_height % com_size != 0) {
        if (com_rank == 0) {
            printf
                ("La tarea no es divisible entre el numero de procesos, %d != 0 (mod %d)\n",
                 win.pixels_height, com_size);
        }
        MPI_Finalize();
        return;
    }

    int buffer_size = win.pixels_width * n_rows;
    printf("Rank: %d, %d %d\n", com_rank, first_row, n_rows);

    uchar *chunk = NULL;
    uchar *axis_chunk = NULL;
    uchar *buffer = NULL;
    double *statistics = NULL;

    if (com_rank == 0) {
        buffer =
            (uchar *) malloc(sizeof(uchar) *
                             (size_t) ((win.pixels_height +
                                        1) * win.pixels_width));
        statistics =
            (double *) malloc(sizeof(double) * (size_t) (com_size));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t = MPI_Wtime();

    chunk = continuous_rows_mandelbrot(win, first_row, n_rows, max_iters);

    axis_chunk =
        mandelbrot_x_axis(win, com_rank * win.pixels_width / com_size,
                          win.pixels_width / com_size, max_iters);
    t = MPI_Wtime() - t;
    MPI_Gather(&t, 1, MPI_DOUBLE, statistics, 1, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
    MPI_Gather(chunk, buffer_size, MPI_UNSIGNED_CHAR, buffer, buffer_size,
               MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    //rebuild the axis
    MPI_Gather(axis_chunk, win.pixels_width / com_size, MPI_UNSIGNED_CHAR,
               (buffer + win.pixels_height / 2 * win.pixels_width),
               win.pixels_width / com_size, MPI_UNSIGNED_CHAR, 0,
               MPI_COMM_WORLD);

    //char path[20];
    //sprintf(path, "%d.pgm", com_rank);
    //write_pgm(path, n_rows, win.pixels_width, 255, chunk);

    free(chunk);

    if (com_rank == 0) {
        int i;
        for (i = 1; i < win.pixels_height / 2; i++) {
            memcpy(buffer +
                   (size_t) (win.pixels_width * (win.pixels_height - i)),
                   buffer + (size_t) (win.pixels_width * i),
                   (size_t) (win.pixels_width));
        }
        char path[100];
        sprintf(path, "mandelbrot_%d_%d.pgm", win.pixels_height,
                max_iters);
        write_pgm(path, win.pixels_height, win.pixels_width, 256, buffer);
        free(buffer);
        char path_statistics[100];
        sprintf(path_statistics, "continuous_statistics_%d_%d_%d_%d",
                win.pixels_width, max_iters, n_rows, com_size);
        freopen(path_statistics, "w", stdout);
        for (int i = 0; i < com_size; i++) {
            printf("%d %f\n", i, statistics[i]);
        }
        free(statistics);
    }

    MPI_Finalize();
}
