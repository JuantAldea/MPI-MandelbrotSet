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
 * task.h
 *
 *  Created on: 17/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef TASK_H_
#define TASK_H_

#include "types.h"

int min(int a, int b);
int max(int a, int b);

void next_task(task current, task last, task next);
void first_assignment(task current, task last, task assignment[], int n);

void set_task(task current_task_assignment[], int rank, task t);
void get_task(task current_task_assignment[], int rank, task t);

int is_last_task(task t);

void print_task(task current_task_assignment[], int com_size);

#endif                          /* TASK_H_ */
