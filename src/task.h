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
