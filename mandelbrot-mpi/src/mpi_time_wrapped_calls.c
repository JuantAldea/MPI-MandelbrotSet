/*
 * mpi_time_wrapped_calls.c
 *
 *  Created on: 16/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */
#include "mpi_time_wrapped_calls.h"
#include <mpi.h>
double mpi_wait_time(MPI_Request *request, MPI_Status *status){
    double t = MPI_Wtime();
    MPI_Wait(request, status);
    return MPI_Wtime() - t;
}

double mpi_isend_time(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request){
    double t = MPI_Wtime();
    MPI_Isend(buf, count, datatype, dest, tag, comm, request);
    return MPI_Wtime() - t;
}

double mpi_irecv_time(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request){
    double t = MPI_Wtime();
    MPI_Irecv(buf, count, datatype, source, tag, comm, request);
    return MPI_Wtime() - t;
}

double mpi_recv_time(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status){
    double t = MPI_Wtime();
    MPI_Recv(buf, count, datatype, source, tag, comm, status);
    return MPI_Wtime() - t;
}

double mpi_send_time(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm){
    double t = MPI_Wtime();
    MPI_Send(buf, count, datatype, dest, tag, comm);
    return MPI_Wtime() - t;
}
