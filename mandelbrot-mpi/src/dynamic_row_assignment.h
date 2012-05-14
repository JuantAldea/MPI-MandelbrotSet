/*
 * dynamic_row_asisgnment.h
 *
 *  Created on: 13/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef DYNAMIC_ROW_ASISGNMENT_H_
#define DYNAMIC_ROW_ASISGNMENT_H_

#include <stdlib.h>

#include "ppm.h"
#include "types.h"
#include "mandelbrot_iteration.h"

void calc_row(window win, int row, int max_iter, uchar *buffer);
void server(window win, int com_size, uchar *image);
void client(window win, int com_root, int max_iter);

void dynamic_row_assignment(int argc, char *argv[]);

#endif /* DYNAMIC_ROW_ASISGNMENT_H_ */
