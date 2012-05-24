/*
 * dynamic2.c
 *
 *  Created on: 17/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#define PRINT_IMAGE
#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "mpi_time_wrapped_calls.h"
#include "dynamic2.h"

void calc_row2(window win, int row, int max_iter, uchar *buffer) {
    float dy = win.y_len /(float) win.pixels_height;
    float dx = win.x_len /(float) win.pixels_width;
    float zi = (float) win.y_start + (float) row * dy;
    float zr;
    for (int i = 0; i < win.pixels_width; i++) {
        zr = (float) win.x_start + (float) i * dx;
        //buffer[i] = ((mandelbrot_iteration(zr, zi, max_iter) == max_iter) ? 255 : 0);
        buffer[i] = (uchar)((mandelbrot_iteration(zr, zi, max_iter)/(float)max_iter) * 255);
    }
}

void client2(window win, int com_root, int com_rank, int max_iter, int task_size) {
    MPI_Status status;
    MPI_Request request;
    task assignment;
    uchar *buffer = (uchar *)malloc(sizeof(uchar) * (size_t)(win.pixels_width * task_size));

    int tasks_solved = 0;
    double receive_time = 0;
    double send_time = 0;
    double wait_time = 0;
    double total_time = MPI_Wtime();

    while(1){

        //printf("Cliente esperando...\n");

        //MPI_Recv(&assignment, 2, MPI_INT, com_root, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        receive_time += mpi_irecv_time(&assignment, 2, MPI_INT, com_root, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        wait_time += mpi_wait_time(&request, &status);

        //printf("Cliente trabajando\n");

        if(status.MPI_TAG == FINISH){
            break;
        }

        for (int i = 0; i < assignment[1]; i++){
            calc_row2(win, assignment[0] + i, max_iter, &buffer[i * win.pixels_width]);
        }

        //printf("Cliente esperando a entregar\n");

        //MPI_Send(buffer, win.pixels_width * assignment[1], MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD);
        send_time += mpi_isend_time(buffer, win.pixels_width * assignment[1], MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD, &request);
        wait_time += mpi_wait_time(&request, &status);
        tasks_solved += assignment[1];
    }
    total_time = MPI_Wtime() - total_time;
    double statistics[6];
    statistics[0] = (double)com_rank;
    statistics[1] = total_time;
    statistics[2] = send_time;
    statistics[3] = receive_time;
    statistics[4] = wait_time;
    statistics[5] = (double)tasks_solved;
    MPI_Gather(statistics, 6, MPI_DOUBLE, NULL, -1, MPI_DOUBLE, com_root, MPI_COMM_WORLD);
    free(buffer);
}

double populate_nodes(task task_assignmet[], int com_size){
    double time = 0;
    MPI_Request request;
    for (int i = 1; i< com_size; i++){
        //MPI_Send(&task_assignmet[i], 2, MPI_INT, i, TASK, MPI_COMM_WORLD);
        time += mpi_isend_time(&task_assignmet[i], 2, MPI_INT, i, TASK, MPI_COMM_WORLD, &request);
    }
    return time;
}

void server2(window win, int com_size, int com_rank, int task_size, uchar *image){

    MPI_Status status;
    MPI_Request send_request, recv_request;
    int tasks_solved = 0;
    int total_tasks = win.pixels_width / 2 + 1;
    uchar *data_buffer = (uchar *) malloc(sizeof(uchar)* (size_t)(win.pixels_width * task_size));
    task  *current_task_assignment = (task *)malloc(sizeof(task) * (size_t)com_size);
    task first, last_task, current_task;
    task_size = min(task_size, total_tasks);
    //set first and last task
    first[0] = 0;
    first[1] = task_size;
    last_task[0] = total_tasks;
    last_task[1] = 0;
    first_assignment(first, last_task, current_task_assignment, com_size);

    double wait_time = 0;
    double send_time = 0;
    double recv_time = 0;
    double total_time = MPI_Wtime();

    send_time += populate_nodes(current_task_assignment, com_size);
    get_task(current_task_assignment, com_size - 1, current_task);
    while(tasks_solved < total_tasks){
        //show the task assignment for each node

        //print_task(current_task_assignment, com_size);
        //printf("Servidor esperando entrega\n");

        //MPI_Recv(data_buffer, win.pixels_width * task_size, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &status);
        recv_time += mpi_irecv_time(data_buffer, win.pixels_width * task_size, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &recv_request);
        wait_time += mpi_wait_time(&recv_request, &status);

        //printf("Servidor recibio entrega\n");

        //save old task info
        task received_data_task;
        get_task(current_task_assignment, status.MPI_SOURCE, received_data_task);
        //send new task
        next_task(current_task, last_task, current_task);
        set_task(current_task_assignment, status.MPI_SOURCE, current_task);
        //printf("Servidor enviando tarea\n");
        if (is_last_task(current_task)){
            //MPI_Send(current_task, 2, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
            send_time += mpi_isend_time(current_task, 2, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD, &send_request);
        }else{
            //MPI_Send(current_task, 2, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD);
            send_time += mpi_isend_time(current_task, 2, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD, &send_request);
        }

        //use old task info to compose the image
        //printf("Servidor componiendo imagen\n");
        //printf("received [%d, %d]\n", received_data_task[0], received_data_task[1]);

#ifdef PRINT_IMAGE
        memcpy(image + received_data_task[0] * win.pixels_width, data_buffer, sizeof(uchar) * (size_t)(win.pixels_width * received_data_task[1]));
        for (int i = 0; i < received_data_task[1]; i++){
            if (received_data_task[0] == 0 && i == 0){
                continue; //ignore row 0 so it has the half row as axis of reflection
            }
            memcpy(image + (win.pixels_height  - received_data_task[0] - i) * win.pixels_width, data_buffer + (i * win.pixels_width), sizeof(uchar) * (size_t)win.pixels_width);
        }

        //char path[20];
        //sprintf(path, "%d-%d.pgm", received_data_task[0], received_data_task[1]);
        //write_pgm(path, win.pixels_height, win.pixels_width, 256, image);
#endif
        tasks_solved += received_data_task[1];

        //printf("Servidor termino de componer\n");
    }

    total_time = MPI_Wtime() - total_time;
    double statistics[6];
    statistics[0] = (double)com_rank;
    statistics[1] = total_time;
    statistics[2] = send_time;
    statistics[3] = recv_time;
    statistics[4] = wait_time;
    statistics[5] = (double)tasks_solved;
    double *full_statistics = (double *)malloc(sizeof(double) * (size_t)(6 * com_size));
    memset(full_statistics, 0, sizeof(int) * (size_t)(6 * com_size));
    MPI_Gather(statistics, 6, MPI_DOUBLE, full_statistics, 6, MPI_DOUBLE, com_rank, MPI_COMM_WORLD);

    printf("#  total     send      recv      wait      tasks\n");
    for (int i = 0; i < com_size; i++){
        printf("%d  %f  %f  %f  %f  %d\n", (int)full_statistics[6*i], full_statistics[6*i+1], full_statistics[i+2],
                full_statistics[6*i+3], full_statistics[6*i+4], (int)full_statistics[6*i+5]);
    }

    free(full_statistics);
    free(data_buffer);
    free(current_task_assignment);
}



void dynamic2(int argc, char *argv[]){
    int max_iters = atoi(argv[3]);
    int task_size = atoi(argv[4]);
    window win;
    win.pixels_height = atoi(argv[2]);
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
        uchar *image = (uchar *)malloc(sizeof(uchar) * (size_t)(win.pixels_height * win.pixels_width));
        memset(image, 128, sizeof(uchar) * (size_t)(win.pixels_height * win.pixels_width));
        server2(win, com_size, com_rank, task_size, image);
        char path[100];
        sprintf(path, "mandelbrot_%d_%d.pgm", win.pixels_height, max_iters);
        write_pgm(path, win.pixels_height, win.pixels_width, 256, image);
        free(image);
    }else{
        client2(win, com_root, com_rank, max_iters, task_size);
    }

    MPI_Finalize();
}
