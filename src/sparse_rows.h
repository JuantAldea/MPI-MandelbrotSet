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
