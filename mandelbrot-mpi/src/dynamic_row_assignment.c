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
#include "mpi_time_wrapped_calls.h"
typedef int task_data[2];

void calc_row(window win, int row, int max_iter, uchar *buffer) {
    double dy = win.y_len /(double) win.pixels_height;
    double dx = win.x_len /(double) win.pixels_width;
    double zi = (double) win.y_start + (double) row * dy;
    double zr;
    int i;
    for (i = 0; i < win.pixels_width; i++) {
        zr = (double) win.x_start + (double) i * dx;
        buffer[i] = ((mandelbrot_iteration(zr, zi, max_iter) == max_iter) ? 255 : 0);
    }
}

void client(window win, int com_root, int max_iter) {
    MPI_Status status;
    MPI_Request request;
    int communication_buffer = 0;
    uchar *buffer = (uchar *)malloc(sizeof(uchar) * win.pixels_width);
    int com_rank;

    int tasks_solved = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);

    double total_time = MPI_Wtime();
    double receive_time = 0;
    double send_time = 0;
    double wait_time = 0;
    while(1){
        receive_time += mpi_irecv_time(&communication_buffer, 1, MPI_INT, com_root, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        wait_time += mpi_wait_time(&request, &status);
        if(status.MPI_TAG == FINISH){
            break;
        }

        calc_row(win, communication_buffer, max_iter, buffer);

        send_time += mpi_isend_time(buffer, win.pixels_width, MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD, &request);
        wait_time += mpi_wait_time(&request, &status);
        tasks_solved++;
    }

    total_time = MPI_Wtime() - total_time;
    printf ("%f, %f, %f, %f, %d\n", total_time, receive_time, send_time, wait_time,tasks_solved);

    free(buffer);
    return;
}

void server(window win, int com_size, uchar *image){
    MPI_Status status;
    int current_task = 0, tasks_solved = 0;
    int total_tasks = win.pixels_width / 2;
    uchar *buffer = (uchar *) malloc(sizeof(uchar)* win.pixels_width);
    int *current_task_asign = (int *) malloc(sizeof(int) * win.pixels_width);
    MPI_Request send_request;
    MPI_Request recv_request;

    double wait_time = 0;
    double send_time = 0;
    double recv_time = 0;
    double total_time = MPI_Wtime();
    int i;
    for (i = 1; i < com_size && i < total_tasks; i++){
        current_task_asign[i] = current_task;
        send_time += mpi_isend_time(&current_task_asign[i], 1, MPI_INT, i, TASK, MPI_COMM_WORLD, &send_request);
        current_task++;
    }

    while(tasks_solved < total_tasks + 1){

        recv_time += mpi_irecv_time(buffer, win.pixels_width, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &recv_request);
        wait_time += mpi_wait_time(&recv_request, &status);

        int previous_task = current_task_asign[status.MPI_SOURCE];
        tasks_solved++;

        if (current_task < total_tasks + 1){
            current_task_asign[status.MPI_SOURCE] = current_task;
            //wait_time += mpi_wait_time(&send_request[i], &status);
            send_time += mpi_isend_time(&current_task_asign[status.MPI_SOURCE], 1, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD, &send_request);
            current_task++;
        }else{
            current_task_asign[status.MPI_SOURCE] = -1;
            //wait_time += mpi_wait_time(&send_request[i], &status);
            send_time += mpi_isend_time(&current_task_asign[status.MPI_SOURCE], 1, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD, &send_request);
        }

        memcpy(image + previous_task * win.pixels_width, buffer, sizeof(uchar) * win.pixels_width);

        if (previous_task != total_tasks && previous_task != 0){
            int reflexion = win.pixels_height - previous_task;
            memcpy(image + reflexion * win.pixels_width, buffer, sizeof(uchar) * win.pixels_width);
        }
    }


    total_time = MPI_Wtime() - total_time;
    printf("%f %f %f\n", total_time, send_time, recv_time);
    free(buffer);
    free(current_task_asign);
}

void dynamic_row_assignment(int argc, char *argv[]){
    int max_iters = atoi(argv[2]);
    window win;
    win.pixels_height = atoi(argv[1]);
    win.pixels_width = win.pixels_height;
    win.x_start = -2;
    win.x_len = 0.8f + 2.f;
    win.y_start = -1.5;
    win.y_len = 1.5 + 1.5;
    int com_rank, com_size;
    int com_root = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &com_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    if (com_rank == com_root){
        uchar *image = (uchar *)malloc(sizeof(uchar) * win.pixels_height * win.pixels_width);
        memset(image, 0, sizeof(uchar) * win.pixels_height * win.pixels_width);
        server(win, com_size, image);
        char path[100];
        sprintf(path, "mandelbrot_%s_%s.ppm", argv[1], argv[2]);
        write_pgm(path, win.pixels_height, win.pixels_width, 256, image);
        free(image);
    }else{
        client(win, com_root, max_iters);
    }

    MPI_Finalize();
}

