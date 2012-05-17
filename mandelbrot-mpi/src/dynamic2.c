/*
 * dynamic2.c
 *
 *  Created on: 17/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#include <mpi.h>
#include <string.h>
#include <stdlib.h>

#include "dynamic2.h"


void client2(window win, int com_root, int com_rank, int max_iter, int task_size) {
    MPI_Status status;
    task assignment;

    uchar *buffer = (uchar *)malloc(sizeof(uchar) * win.pixels_width * task_size);

    while(1){
        MPI_Recv(&assignment, 2, MPI_INT, com_root, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if(status.MPI_TAG == FINISH){
            break;
        }

        for (int i = 0; i < assignment[1]; i++){
            calc_row(win, assignment[0] + i, max_iter, &buffer[i * win.pixels_width]);
        }
        MPI_Send(buffer, win.pixels_width * assignment[1], MPI_UNSIGNED_CHAR, com_root, DATA, MPI_COMM_WORLD);
    }

}

void populate_nodes(task task_assignmet[], int com_size){
    for (int i = 1; i< com_size; i++){
        MPI_Send(&task_assignmet[i], 2, MPI_INT, i, TASK, MPI_COMM_WORLD);
    }
}

void server2(window win, int com_size, int com_rank, int task_size, uchar *image){
    MPI_Status status;

    int tasks_solved = 0;
    int total_tasks = win.pixels_width / 2 + 1;
    uchar *data_buffer = (uchar *) malloc(sizeof(uchar)* win.pixels_width * task_size);
    task  *current_task_assignment = (task *)malloc(sizeof(task) * com_size);
    task first, last_task;
    first[0] = 0;
    first[1] = task_size;
    last_task[0] = total_tasks;
    last_task[1] = 0;
    first_assignment(first, last_task, current_task_assignment, com_size);

    populate_nodes(current_task_assignment, com_size);
    task current_task;
    get_task(current_task_assignment, com_size - 1, current_task);
    while(tasks_solved < total_tasks){
        print_task(current_task_assignment, com_size);
        MPI_Recv(data_buffer, win.pixels_width * task_size, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, DATA, MPI_COMM_WORLD, &status);
        //SAVE OLD TASK INFO
        task received_data_task;
        get_task(current_task_assignment, status.MPI_SOURCE, received_data_task);
        //SEND NEW TASK
        next_task(current_task, last_task, current_task);
        set_task(current_task_assignment, status.MPI_SOURCE, current_task);
        if (is_last_task(current_task)){
            MPI_Send(current_task, 2, MPI_INT, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
        }else{
            MPI_Send(current_task, 2, MPI_INT, status.MPI_SOURCE, TASK, MPI_COMM_WORLD);
        }
        //USE OLD TASK INFO TO COMPOSE THE IMAGE
        printf("recibida [%d, %d]\n", received_data_task[0], received_data_task[1]);

        memcpy(image + received_data_task[0] * win.pixels_width, data_buffer, sizeof(uchar) * win.pixels_width * received_data_task[1]);
        for (int i = 0; i < received_data_task[1]; i++){
            if (received_data_task[0] == 0 && i == 0){
                continue; //ignore row 0
            }
            memcpy(image + (win.pixels_height  - received_data_task[0] - i) * win.pixels_width, data_buffer + (i * win.pixels_width), sizeof(uchar) * win.pixels_width);
        }

        char path[20];
        sprintf(path, "%d-%d.pgm", received_data_task[0], received_data_task[1]);
        write_pgm(path, win.pixels_height, win.pixels_width, 256, image);
        tasks_solved += received_data_task[1];
    }
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
        system("rm -f *pgm");
        system("rm -f *ppm");
        printf("Somos %d procesos\n", com_size);
        uchar *image = (uchar *)malloc(sizeof(uchar) * win.pixels_height * win.pixels_width);
        memset(image, 0, sizeof(uchar) * win.pixels_height * win.pixels_width);
        server2(win, com_size, com_rank, task_size, image);
        char path[100];
        sprintf(path, "mandelbrot_%s_%s.pgm", win.pixels_height, max_iters);
        write_pgm(path, win.pixels_height, win.pixels_width, 256, image);

        free(image);
    }else{
        client2(win, com_root, com_rank, max_iters, task_size);
    }

    MPI_Finalize();
}
