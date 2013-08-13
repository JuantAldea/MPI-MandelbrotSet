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
 * ppm.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */


#include "ppm.h"

int write_pgm(char *path, int height, int width, int grey_levels,
              uchar data[])
{
    FILE *file = fopen(path, "w");

    if (!file) {
        printf("Unable to open %s.\n", path);
        return -1;
    }

    fprintf(file, "%s\n%d %d\n%d\n", "P2", width, height, grey_levels - 1);

    int i, j;
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            fprintf(file, "%d ", data[j * width + i]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}

int write_ppm(char *path, int height, int width, int color_levels,
              uchar data[])
{
    FILE *file = fopen(path, "w");

    if (!file) {
        printf("Unable to open %s.\n", path);
        return -1;
    }

    fprintf(file, "%s\n%d %d\n%d\n", "P3", width, height,
            color_levels - 1);

    int i, j;
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            fprintf(file, "%d %d %d  ", data[j * 3 * width + 3 * i + 0],
                    data[j * 3 * width + 3 * i + 1],
                    data[j * 3 * width + 3 * i + 2]);
            fprintf(file, "\n");
        }

    }

    fclose(file);

    return 0;
}
