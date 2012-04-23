/*
 * sparse_rows.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "sparse_rows.h"
#include "math_optimizations.h"

int *sparse_rows_mandelbrot(window win, int first_row, int n_rows, int row_step, int max_iter) {

    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float zr, zi, cr, ci, zrs, zis;
    int i, j, color;
    i = j = -1;
    int *image = (int *) malloc(sizeof(int) * n_rows * win.pixels_width);

    //for (j = row; j < row + rowdim; j++) {
    for (j = first_row; j < win.pixels_height; j += row_step) {
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
            image[(j - first_row) / row_step * win.pixels_width + i] = 255;
        }
    }
    return image;
}

int *sparse_rows_image_reconstruction(int n_threads, int pixels_height, int pixels_width, int buffer[]) {
    int j;
    int *output = (int *) malloc(sizeof(int) * pixels_height * pixels_width);
    for (j = 0; j < pixels_height; j++) {
        memcpy(output + j * pixels_width, buffer + ((j % n_threads) * pixels_height / n_threads + j / n_threads) * pixels_width,
                sizeof(int) * pixels_width);
    }
    return output;
}
