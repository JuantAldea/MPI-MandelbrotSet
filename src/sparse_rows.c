/*
Copyright (C) 2012, Juan Antonio Aldea Armenteros
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * sparse_rows.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

#include "ppm.h"
#include "mandelbrot_iteration.h"

#include "sparse_rows.h"

uchar *sparse_rows_mandelbrot(window win, int first_row, int n_rows,
                              int row_step, int max_iter)
{

    float dy = win.y_len / (float) win.pixels_height;
    float dx = win.x_len / (float) win.pixels_width;
    float cr, ci;
    int i, j;

    uchar *image =
        (uchar *) malloc(sizeof(uchar) * n_rows * win.pixels_width);

    for (j = first_row; j < win.pixels_height; j += row_step) {
        for (i = 0; i < win.pixels_width; i++) {
            ci = (float) win.y_start + (float) j *dy;
            cr = (float) win.x_start + (float) i *dx;
            image[(j - first_row) / row_step * win.pixels_width + i] =
                (mandelbrot_iteration(cr, ci, max_iter) ==
                 max_iter) ? 255 : 0;
        }
    }
    return image;
}

uchar *sparse_rows_image_reconstruction(int n_threads, int pixels_height,
                                        int pixels_width, uchar buffer[])
{
    int j;
    uchar *output =
        (uchar *) malloc(sizeof(uchar) * pixels_height * pixels_width);
    for (j = 0; j < pixels_height; j++) {
        memcpy(output + j * pixels_width,
               buffer + ((j % n_threads) * pixels_height / n_threads +
                         j / n_threads) * pixels_width,
               sizeof(uchar) * pixels_width);
    }
    return output;
}

void sparse_rows_version(int argc, char *argv[])
{
    window win;
    int max_iters = atoi(argv[3]);
    win.pixels_height = atoi(argv[2]);
    win.pixels_width = win.pixels_height;
    win.x_start = -2;
    win.x_len = 0.8f + 2.0f;
    win.y_start = -1.5;
    win.y_len = 1.5 + 1.5;

    int com_rank, com_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    uchar *chunk = NULL;
    uchar *buffer = NULL;

    printf("Rank: %d, %d %d\n", com_rank,
           com_rank * win.pixels_height / com_size,
           win.pixels_height / com_size);

    chunk =
        sparse_rows_mandelbrot(win, com_rank, win.pixels_height / com_size,
                               com_size, max_iters);

    int buffer_size = win.pixels_width * win.pixels_height / com_size;

    if (com_rank == 0) {
        buffer =
            (uchar *) malloc(sizeof(uchar) * win.pixels_height *
                             win.pixels_width);
    }

    MPI_Gather(chunk, buffer_size, MPI_UNSIGNED_CHAR, buffer, buffer_size,
               MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    char path[20];
    sprintf(path, "%d.pgm", com_rank);

    write_pgm(path, win.pixels_height, win.pixels_width / com_size, 255,
              chunk);

    free(chunk);

    if (com_rank == 0) {
        uchar *ordered_im =
            sparse_rows_image_reconstruction(com_size, win.pixels_height,
                                             win.pixels_width, buffer);
        char path[100];
        sprintf(path, "mandelbrot_%d_%d.pgm", win.pixels_height,
                max_iters);
        write_pgm(path, win.pixels_height, win.pixels_width, 255,
                  ordered_im);
        free(ordered_im);
        free(buffer);
    }

    MPI_Finalize();
}
