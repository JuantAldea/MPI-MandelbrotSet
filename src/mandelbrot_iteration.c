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
 * mandelbrot_iteration.c
 *
 *  Created on: 14/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include "math_optimizations.h"

int mandelbrot_iteration(float cr, float ci, int max_iter)
{
    float zi, zr, zrs, zis;
    zi = ci;
    zr = cr;
    zrs = zis = (float) 0;
    int n_iter = 0;

    if (test_point_in_cardiod_or_2ndbud(zr, zi)) {
        n_iter = max_iter;
    } else {
        while (zrs + zis < (float) 4 && n_iter < max_iter) {
            zrs = zr * zr;
            zis = zi * zi;
            zi = (float) 2 *zr * zi + ci;
            zr = zrs - zis + cr;
            n_iter++;
        }
    }
    return n_iter;
}
