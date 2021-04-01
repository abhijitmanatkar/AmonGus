#include "main.h"

#ifndef GRID_H
#define GRID_H

#define SIZE 15

void init_maze(char maze[SIZE][SIZE]);
void partition(char maze[SIZE][SIZE], int left, int right, int top, int bottom);
void min_distances(char maze[SIZE][SIZE], int dist[SIZE][SIZE][SIZE][SIZE]);

#endif