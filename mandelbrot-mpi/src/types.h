/*
 * window.h
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned char uchar;

typedef struct {
    int pixels_height;
    int pixels_width;
    float y_start;
    float x_start;
    float y_len;
    float x_len;
} window;

#endif /* TYPES_H_ */
