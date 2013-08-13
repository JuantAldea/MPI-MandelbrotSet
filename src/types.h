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
 * window.h
 *
 *  Created on: 12/05/2012
 *      Author: Juan Antonio Aldea Armenteros
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned char uchar;

typedef struct {
    int pixels_height;
    int pixels_width;
    float y_start;
    float x_start;
    float y_len;
    float x_len;
} window;

typedef enum { TASK = 0, FINISH, TASK_REQUEST, DATA } msg_tag;

typedef int task[2];

#endif                          /* TYPES_H_ */
