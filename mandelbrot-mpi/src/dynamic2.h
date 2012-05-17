/*
 * dynamic2.h
 *
 *  Created on: 17/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef DYNAMIC2_H_
#define DYNAMIC2_H_

#include <stdlib.h>


#include "task.h"
#include "ppm.h"
#include "types.h"
#include "mandelbrot_iteration.h"

void client2(window win, int com_root, int com_rank, int max_iter, int task_size);
void server2(window win, int com_size, int com_rank, int task_size, uchar *image);
void populate_nodes(task task_assignmet[], int com_size);
void calc_row2(window win, int row, int max_iter, uchar *buffer);

void dynamic2(int argc, char *argv[]);

#endif /* DYNAMIC2_H_ */
