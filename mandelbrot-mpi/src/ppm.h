/*
 * ppm.h
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef PPM_H_
#define PPM_H_

int write_pgm(char *path, int height, int width, int grey_levels, unsigned char data[]);
int write_ppm(char *path, int height, int width, int grey_levels, unsigned char data[]);

#endif /* PPM_H_ */
