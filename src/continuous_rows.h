/*
 * continuous_rows.h
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef CONTINUOUS_ROWS_H_
#define CONTINUOUS_ROWS_H_

#include "types.h"

uchar *mandelbrot_x_axis(window win, int first_pixel, int n_pixels,
                         int max_iter);
uchar *continuous_rows_mandelbrot(window win, int first_row, int n_rows,
                                  int max_iter);

void continuous_rows_version(int argc, char *argv[]);

#endif                          /* CONTINUOUS_ROWS_H_ */
