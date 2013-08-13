/*
Copyright (C) 2012, Juan Antonio Aldea Armenteros
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * mpi_time_wrapped_calls.c
 *
 *  Created on: 16/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */
#include "mpi_time_wrapped_calls.h"
#include <mpi.h>
double mpi_wait_time(MPI_Request * request, MPI_Status * status)
{
    double t = MPI_Wtime();
    MPI_Wait(request, status);
    return MPI_Wtime() - t;
}

double mpi_isend_time(void *buf, int count, MPI_Datatype datatype,
                      int dest, int tag, MPI_Comm comm,
                      MPI_Request * request)
{
    double t = MPI_Wtime();
    MPI_Isend(buf, count, datatype, dest, tag, comm, request);
    return MPI_Wtime() - t;
}

double mpi_irecv_time(void *buf, int count, MPI_Datatype datatype,
                      int source, int tag, MPI_Comm comm,
                      MPI_Request * request)
{
    double t = MPI_Wtime();
    MPI_Irecv(buf, count, datatype, source, tag, comm, request);
    return MPI_Wtime() - t;
}

double mpi_recv_time(void *buf, int count, MPI_Datatype datatype,
                     int source, int tag, MPI_Comm comm,
                     MPI_Status * status)
{
    double t = MPI_Wtime();
    MPI_Recv(buf, count, datatype, source, tag, comm, status);
    return MPI_Wtime() - t;
}

double mpi_send_time(void *buf, int count, MPI_Datatype datatype, int dest,
                     int tag, MPI_Comm comm)
{
    double t = MPI_Wtime();
    MPI_Send(buf, count, datatype, dest, tag, comm);
    return MPI_Wtime() - t;
}
