/*
 * dynamic_row_asisgnment.h
 *
 *  Created on: 13/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include "dynamic_row_assignment.h"

typedef unsigned char uchar;
/*
int calc_point(int cr, int ci, int max_iter) {
    float zi, zr, zrs, zis;
    zi = ci;
    zr = cr;
    zrs = zis = (float) 0;
    int color = 0;

    if (test_point_in_cardiod_or_2ndbud(zr, zi)) {
        color = max_iter;
    } else {
        while (zrs + zis < (float) 4 && color < max_iter) {
            zrs = zr * zr;
            zis = zi * zi;
            zi = (float) 2 * zr * zi + ci;
            zr = zrs - zis + cr;
            color++;
        }
    }
    return color;
}

void calc_row(window win, int row, int max_iter, uchar *buffer) {
    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float zi = (float) win.y_start + (float) row * dy;
    float zr;
    int i;
    for (i = 0; i < win.pixels_width; i++) {
        zr = (float) win.x_start + (float) i * dx;
        buffer[i] = calc_point(zr, zi, max_iter);
    }
}

void client(window win) {
    int row = 1024;
    int max_iter = 100;
    uchar *row_colors = (uchar *) malloc(sizeof(uchar) * win.pixels_width);
    //MPI_Recv()
    calc_row(win, row, max_iter, row_colors);
    //MPI_Send()
}

void server(){
    int tasks_left = 1024
    //MPI_Recv()
    //si llega una peticion
        // aun hay tareas disponibles
            //enviar datos
            //MPI_send()s
        // si no quedan
            //enviar kill

    //si llegan datos
        //escribir datos o ver si se puede dejar ese mensaje a la espera
    //si
}
*/
