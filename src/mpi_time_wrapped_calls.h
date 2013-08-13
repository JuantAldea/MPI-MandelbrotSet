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
 * mpi_time_wrapped_calls.h
 *
 *  Created on: 16/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef MPI_TIME_WRAPPED_CALLS_H_
#define MPI_TIME_WRAPPED_CALLS_H_

#include <mpi.h>

double mpi_wait_time(MPI_Request * request, MPI_Status * status);

double mpi_isend_time(void *buf, int count, MPI_Datatype datatype,
                      int dest, int tag, MPI_Comm comm,
                      MPI_Request * request);
double mpi_irecv_time(void *buf, int count, MPI_Datatype datatype,
                      int source, int tag, MPI_Comm comm,
                      MPI_Request * request);

double mpi_recv_time(void *buf, int count, MPI_Datatype datatype,
                     int source, int tag, MPI_Comm comm,
                     MPI_Status * status);
double mpi_send_time(void *buf, int count, MPI_Datatype datatype, int dest,
                     int tag, MPI_Comm comm);

#endif                          /* MPI_TIME_WRAPPED_CALLS_H_ */
