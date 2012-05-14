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
    uchar *row_colors = (uchar *) malloc(sizeof(uchar) * win.pixels_width);

    while(1){
        MPI_Send(&communication_buffer, 1, MPI_INT, com_root, TASK_REQUEST, MPI_COMM_WORLD);

        MPI_Recv(&communication_buffer, 1, MPI_INT, com_root, MPI_ANY_TAG,  MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == FINISH) {
            break;
        }
        printf ("calculando %d\n", communication_buffer);
        calc_row(win, communication_buffer, max_iter, row_colors);

        MPI_Send(&row_colors, win.pixels_width, MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD);
        printf ("enviando %d\n", communication_buffer);
    }

    free(row_colors);
    return;
}

void server(window win, int com_size, uchar *image){
    MPI_Status status;
    printf ("Numero de clientes: %d\n", com_size - 1);
    int *current_taks_assign = (int *)malloc (sizeof(int) * com_size);
    if (current_taks_assign == NULL){
        printf("ES NULL\n");
    }
    int i;
    for (i = 0; i < com_size; i++){
        current_taks_assign[i] = -1;
    }
    printf("#################################\n");
    void *buffer = (void *) malloc(sizeof(uchar) * win.pixels_width);
    uchar *buffer_data = NULL;
    int communication_buffer;

    int next_task = win.pixels_height / 2;
    int writes_pending = win.pixels_height / 2;

    while (writes_pending > 0){
        MPI_Recv(&communication_buffer, win.pixels_width, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG,  MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == DATA) {
            /*
            buffer_data = (uchar *)buffer;
            memcpy(image + win.pixels_width * current_taks_assign[status.MPI_SOURCE - 1], buffer_data, sizeof(uchar) * win.pixels_width);
            if (current_taks_assign[status.MPI_SOURCE - 1] != win.pixels_height / 2){
                memcpy(image + win.pixels_width *(win.pixels_height - current_taks_assign[status.MPI_SOURCE - 1]),
                        buffer_data, sizeof(uchar) * win.pixels_width);
            }
            */
            //current_taks_assign[status.MPI_SOURCE] = -1;//change status to free
            //writes_pending--;
        }else if (status.MPI_TAG == TASK_REQUEST){

            if (next_task >= 0){
                printf("ANTES %d\n", status.MPI_SOURCE);
                current_taks_assign[status.MPI_SOURCE] = next_task; //<<--------- ESTA REVENTANDO AQUI
                printf("Peticion de %d -> %d\n", status.MPI_SOURCE, next_task);
                MPI_Send(&next_task, 1, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD);
                next_task--;
            }else{
                printf("Peticion de %d -> ACABAR\n", status.MPI_SOURCE);
                MPI_Send(&next_task, 1, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
            }
        }
    }
    free(buffer);
}

void dynamic_row_assignment(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int com_rank, com_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    window win;
    int max_iters = atoi(argv[2]);
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
        write_pgm("broza.ppm", win.pixels_height, win.pixels_width, 255, image);
    }else{
        client(win, com_root, max_iters);

    }
    MPI_Finalize();
}
