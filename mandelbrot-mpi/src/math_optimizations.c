/*
 * math_optimizations.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include "math_optimizations.h"

inline int test_point_in_cardiod_or_2ndbud(float x, float y) {
    //float q = (x - 0.25) * (x - 0.25) + y * y;
    //return q * (q + x - 0.25) < 0.25 * y * y;
    x = x - 0.25;
    y = y * y;
    float q = x * x + y;
    return (q * (q + x)) < (0.25 * y);
}
