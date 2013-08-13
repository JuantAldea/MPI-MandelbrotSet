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
