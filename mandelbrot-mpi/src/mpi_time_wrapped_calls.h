/*
 * mpi_time_wrapped_calls.h
 *
 *  Created on: 16/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef MPI_TIME_WRAPPED_CALLS_H_
#define MPI_TIME_WRAPPED_CALLS_H_

#include <mpi.h>

double mpi_wait_time(MPI_Request *request, MPI_Status *status);

double mpi_isend_time(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
double mpi_irecv_time(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);

double mpi_recv_time (void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
double mpi_send_time (void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

#endif /* MPI_TIME_WRAPPED_CALLS_H_ */
