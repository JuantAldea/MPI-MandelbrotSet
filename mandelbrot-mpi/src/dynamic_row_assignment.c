/*
 * dynamic_row_asisgnment.h
 *
 *  Created on: 13/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <stdio.h>
#include <mpi.h>
#include <string.h>

#include "ppm.h"
#include "dynamic_row_assignment.h"

void calc_row(window win, int row, int max_iter, uchar *buffer) {
    float dy = win.y_len / win.pixels_height;
    float dx = win.x_len / win.pixels_width;
    float zi = (float) win.y_start + (float) row * dy;
    float zr;

    int i;
    for (i = 0; i < win.pixels_width; i++) {
        zr = (float) win.x_start + (float) i * dx;
        buffer[i] = ((mandelbrot_iteration(zr, zi, max_iter) == max_iter) ? 255 : 0);
    }
}

void client(window win, int com_root, int max_iter) {
    MPI_Status status;
    int communication_buffer = 0;
    uchar *buffer = (uchar *)malloc(sizeof(uchar) * win.pixels_width);
    int com_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);

    while(1){
        MPI_Recv(&communication_buffer, 1, MPI_INT, com_root, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == FINISH){
            break;
        }

        calc_row(win, communication_buffer, max_iter, buffer);
        MPI_Send(buffer, win.pixels_width, MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD);
    }
    free(buffer);
    return;
}

void server(window win, int com_size, uchar *image){
    MPI_Status status;
    int current_task = 0, tasks_solved = 0;

    uchar *buffer = (uchar *) malloc(sizeof(uchar)* win.pixels_width);
    int *current_task_asign = (int *) malloc(sizeof(int) * win.pixels_width);

    int i;
    for (i = 1; i < com_size && i < win.pixels_width; i++){
        MPI_Send(&current_task, 1, MPI_INT, i, TASK, MPI_COMM_WORLD);
        current_task_asign[i] = current_task;
        current_task++;
    }

    while(tasks_solved < win.pixels_width){
        MPI_Recv(buffer, win.pixels_width, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &status);
        memcpy(image + current_task_asign[status.MPI_SOURCE] * win.pixels_width, buffer, sizeof(uchar) * win.pixels_width);
        current_task_asign[status.MPI_SOURCE] = -1;
        tasks_solved++;

        if (current_task < win.pixels_width){
            MPI_Send(&current_task, 1, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD);
            current_task_asign[status.MPI_SOURCE] = current_task;
            current_task++;
        }else{
            MPI_Send(&current_task, 1, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
        }
    }
    free(buffer);
    free(current_task_asign);
}

void dynamic_row_assignment(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int com_rank, com_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    int max_iters = atoi(argv[2]);

    window win;
    win.pixels_height = atoi(argv[1]);
    win.pixels_width = win.pixels_height;
    win.x_start = -2;
    win.x_len = 0.8 + 2;
    win.y_start = -1.5;
    win.y_len = 1.5 + 1.5;

    int com_root = 0;

    if (com_rank == com_root){
        uchar *image = (uchar *)malloc(sizeof(uchar) * win.pixels_height * win.pixels_width);
        server(win, com_size, image);
        write_pgm("broza.ppm", win.pixels_height, win.pixels_width, 16, image);
        free(image);
    }else{
        client(win, com_root, max_iters);
    }
    MPI_Finalize();
}

