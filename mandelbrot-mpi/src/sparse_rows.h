/*
 * sparse_rows.h
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef SPARSE_ROWS_H_
#define SPARSE_ROWS_H_

typedef struct {
    int pixels_height;
    int pixels_width;
    float y_start;
    float x_start;
    float y_len;
    float x_len;
} window;

unsigned char *sparse_rows_mandelbrot(window win, int first_row, int n_rows, int row_step, int max_iter);

unsigned char *sparse_rows_image_reconstruction(int n_threads, int pixels_height, int pixels_width, unsigned char buffer[]);

#endif /* SPARSE_ROWS_H_ */
