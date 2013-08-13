/*
 * sparse_rows.h
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef SPARSE_ROWS_H_
#define SPARSE_ROWS_H_

#include "types.h"

uchar *sparse_rows_mandelbrot(window win, int first_row, int n_rows,
                              int row_step, int max_iter);

uchar *sparse_rows_image_reconstruction(int n_threads, int pixels_height,
                                        int pixels_width, uchar buffer[]);

void sparse_rows_version(int argc, char *argv[]);

#endif                          /* SPARSE_ROWS_H_ */
