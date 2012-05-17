/*
 * mandelbrot.c
 *
 *  Created on: 22/04/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdlib.h>

#include "algorithms.h"

int main(int argc, char* argv[]) {
    system("rm -f *pgm");
    system("rm -f *ppm");
    switch(atoi(argv[1])){
        case 0:
            sparse_rows_version(argc, argv);
            break;
        case 1:
            continuous_rows_version(argc, argv);
            break;
        case 2:
            dynamic_row_assignment(argc, argv);
            break;
        case 3:
            dynamic2(argc, argv);
            break;
    }

    return EXIT_SUCCESS;
}

