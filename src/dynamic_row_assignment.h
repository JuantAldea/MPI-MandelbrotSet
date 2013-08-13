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

void calc_row(window win, int row, int max_iter, uchar * buffer);

void server(window win, int com_size, int com_rank, uchar * image);
void client(window win, int com_root, int com_rank, int max_iter);

void dynamic_row_assignment(int argc, char *argv[]);

#endif                          /* DYNAMIC_ROW_ASISGNMENT_H_ */
