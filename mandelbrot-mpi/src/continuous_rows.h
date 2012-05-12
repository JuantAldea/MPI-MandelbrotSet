/*
 * continuous_rows.h
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef CONTINUOUS_ROWS_H_
#define CONTINUOUS_ROWS_H_

#include "window.h"

unsigned char *continuous_rows_mandelbrot(window win, int first_row, int n_rows, int max_iter);

void continuous_rows_version(int argc, char *argv[]);

#endif /* CONTINUOUS_ROWS_H_ */
