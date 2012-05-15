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
        printf("CLIENTE ESPERANDO ORDENES\n");
        MPI_Recv(&communication_buffer, 1, MPI_INT, com_root, MPI_ANY_TAG,  MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == FINISH) {
            printf("Me mandan TERMINAR\n");
            break;
        }
        printf ("Cliente calculando %d\n", communication_buffer);
        //calc_row(win, communication_buffer, max_iter, row_colors);
        int i;
       memset(row_colors, 1, sizeof(uchar) * win.pixels_width);
        for (i = 0; i < win.pixels_width; i++){
            printf ("%d ", row_colors[i]);
        }
        MPI_Send(&row_colors, win.pixels_width, MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD);
        for (i = 0; i < win.pixels_width; i++){
            printf ("%d ", row_colors[i]);
        }
        printf ("Cliente enviado %d\n", communication_buffer);
    }
    printf ("##################Soy cliente y he terminado\n");
    free(row_colors);
    return;
}

void server(window win, int com_size, uchar *image){
    MPI_Status status;
    printf ("Numero de clientes: %d\n", com_size - 1);
    int *current_taks_assign = (int *)malloc (sizeof(int) * com_size);


    uchar *buffer = (uchar *) malloc(sizeof(uchar) * win.pixels_width);
    int next_task = 0;
    int tasks_completed = 0;
    int i;

    memset(buffer, 2, sizeof(uchar) * win.pixels_width);
    for (i = 0; i < win.pixels_width; i++){
        printf ("%d ", buffer[i]);
    }
    printf("\n");

    for (i = 1; i < com_size && i < win.pixels_height; i++){
        printf("Enviado tarea inicial %d -> %d\n", i, next_task);
        MPI_Send(&next_task, 1, MPI_INT, i, TASK, MPI_COMM_WORLD);
        current_taks_assign[i] = next_task;
        next_task++;
    }
    printf("#################################\n");
    while (tasks_completed < win.pixels_width){
        printf("Voy por la tarea %d y he recibido %d filas\n", next_task, tasks_completed);
        //MPI_Recv(buf,    count,            datatype,          source,         tag,  comm,            status);
        MPI_Recv(buffer, win.pixels_width, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &status);
        if(status.MPI_ERROR){
            printf("ERROR\n");
            exit(-1);
        }
        printf("Recibo comunicacion de %d\n", status.MPI_SOURCE);
        if (status.MPI_TAG == DATA) {
            printf("\tHe recibo datos de %d\n", status.MPI_SOURCE);
            printf("\t\tProceso los datos de %d\n", status.MPI_SOURCE);
            memcpy(image, buffer, sizeof(uchar) * win.pixels_width);
            for (i = 0; i < win.pixels_width; i++){
                printf ("%d %d ", image[i], buffer[i]);
            }
            printf("\n");
            current_taks_assign[status.MPI_SOURCE] = -1;//change status to free
            tasks_completed++;
            if (next_task < win.pixels_height){
                 printf("\t\t quedan tareas, asi que le envio a %d la tarea %d\n", status.MPI_SOURCE, next_task);
                 MPI_Send(&next_task, 1, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD);
                 current_taks_assign[status.MPI_SOURCE] = next_task;
                 next_task++;
             }else{
                 printf("\t\t NO quedan tareas, se manda FIN\n");
                 MPI_Send(&next_task, 1, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
             }
            printf("\tFin del procesamiento de los datos recibidos\n");
        }
    }
    printf("SERVIDOR TERMINA\n");
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
       int i;
       for (i = 0; i < win.pixels_height * win.pixels_width; i++){
           image[i] = 128;
       }
        server(win, com_size, image);
        printf("servidor vuelve al main\n");
        write_pgm("broza.ppm", win.pixels_height, win.pixels_width, 255, image);
    }else{
        client(win, com_root, max_iters);
        printf("cliente vuelve al main\n");
    }
    MPI_Finalize();
}

