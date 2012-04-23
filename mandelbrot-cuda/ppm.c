/*
 * ppm.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdio.h>
#include "ppm.h"

int write_pgm(char *path, int height, int width, int grey_levels, unsigned char data[]) {
	FILE *file = fopen(path, "w");

	if (!file) {
		printf("Unable to open %s.\n", path);
		return -1;
	}

	//write header
	fprintf(file, "%s\n%d %d\n%d\n", "P2", width, height, grey_levels);

	int i, j;
	//write data
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			fprintf(file, "%d ", data[j * width + i]);
		}
		fprintf(file, "\n");
	}

	fclose(file);

	return 0;
}

int write_ppm(char *path, int height, int width, int grey_levels, unsigned char data[]) {
	FILE *file = fopen(path, "w");

	if (!file) {
		printf("Unable to open %s.\n", path);
		return -1;
	}

	//write header
	fprintf(file, "%s\n%d %d\n%d\n", "P3", width, height, grey_levels);

	int i, j;
	//write data
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			fprintf(file, "%d %d %d  ",
					data[j * 3 * width + 3 * i + 0],
					data[j * 3 * width + 3 * i + 1],
					data[j * 3 * width + 3 * i + 2]);
			fprintf(file, "\n");
		}

	}

	fclose(file);

	return 0;
}
