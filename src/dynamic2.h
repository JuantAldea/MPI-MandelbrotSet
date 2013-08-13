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

void client2(window win, int com_root, int com_rank, int max_iter,
             int task_size);
void server2(window win, int com_size, int com_rank, int task_size,
             uchar * image, int max_iters);
void calc_row2(window win, int row, int max_iter, uchar * buffer);
double populate_nodes(task task_assignmet[], int com_size);

void dynamic2(int argc, char *argv[]);

#endif                          /* DYNAMIC2_H_ */
