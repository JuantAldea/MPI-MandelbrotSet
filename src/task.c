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
 * task.c
 *
 *  Created on: 17/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */
#include <stdio.h>
#include <string.h>
#include "task.h"

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int max(int a, int b)
{
    return (a < b) ? b : a;
}

void next_task(task current, task last, task next)
{
    next[0] = current[0] + current[1];
    next[1] = min(last[0] - next[0], current[1]);
}

void first_assignment(task current, task last, task assignment[], int n)
{
    memset(assignment, 0, sizeof(task) * n);
    if (n == 0) {
        return;
    }

    memcpy(&assignment[1], current, sizeof(task));
    for (int i = 2; i < n; i++) {
        next_task(assignment[i - 1], last, assignment[i]);
    }
}

void set_task(task current_task_assignment[], int rank, task t)
{
    memcpy(&current_task_assignment[rank], t, sizeof(task));
}

void get_task(task current_task_assignment[], int rank, task t)
{
    memcpy(t, &current_task_assignment[rank], sizeof(task));
}

void print_task(task current_task_assignment[], int com_size)
{
    for (int i = 0; i < com_size; i++) {
        printf("[%d, %d]", current_task_assignment[i][0],
               current_task_assignment[i][1]);
    }
    printf("\n");
}

int is_last_task(task t)
{
    return t[1] == 0;
}
