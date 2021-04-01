#include "grid.h"
#define INF 1e9


void print_maze(char maze[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

void init_maze(char maze[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            maze[i][j] = ' ';
            if (i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1)
                maze[i][j] = 'X';
        }
    }
}


void partition(char maze[SIZE][SIZE], int left, int right, int top, int bottom)
{
    int i, hole, choice;
    int hpart = -1;
    int vpart = -1;

    if (right - left > 5)
        hpart = left + 2 + (rand() % (right - left - 3));

    if (bottom - top > 5)
        vpart = top + 2 + (rand() % (bottom - top - 3));

    if (hpart != -1)
    {
        for (i = top; i <= bottom; i++)
        {
            maze[i][hpart] = 'X';
        }
    }

    if (vpart != -1)
    {
        for (i = left; i <= right; i++)
        {
            maze[vpart][i] = 'X';
        }
    }

    if (hpart == -1 && vpart == -1)
        return;

    if (hpart == -1 && vpart != -1)
    {
        hole = left + 1 + (rand() % (right - left - 1));
        maze[vpart][hole] = ' ';
        partition(maze, left, right, top, vpart);
        partition(maze, left, right, vpart, bottom);
    }

    else if (vpart == -1 && hpart != -1)
    {
        hole = top + 1 + (rand() % (bottom - top - 1));
        maze[hole][hpart] = ' ';
        partition(maze, left, hpart, top, bottom);
        partition(maze, hpart, right, top, bottom);
    }

    else
    {
        hole = top + 1 + (rand() % (vpart - top - 1));
        maze[hole][hpart] = ' ';

        hole = vpart + 1 + (rand() % (bottom - vpart - 1));
        maze[hole][hpart] = ' ';

        hole = left + 1 + (rand() % (hpart - left - 1));
        maze[vpart][hole] = ' ';

        hole = hpart + 1 + (rand() % (right - hpart - 1));
        maze[vpart][hole] = ' ';

        partition(maze, left, hpart, top, vpart);
        partition(maze, hpart, right, top, vpart);
        partition(maze, left, hpart, vpart, bottom);
        partition(maze, hpart, right, vpart, bottom);
    }
    // Check that region has atleast one exit
    bool hasExit = false;
    for(i = left; i<=right; i++){
        if(maze[top][i] == ' ' || maze[bottom][i] == ' '){
            hasExit = true;
            break;
        }
    }
    for(i = top; i<=bottom; i++){
        if(maze[i][left] == ' ' || maze[i][right] == ' '){
            hasExit = true;
            break;
        }
    }
    if(!hasExit){
        if (left != 0){
            hole = top + 1 + (rand() % (bottom - top -1));
            maze[hole][left] = ' ';
            return;
        }
        if (right != SIZE-1){
            hole = top + 1 + (rand() % (bottom - top -1));
            maze[hole][right] = ' ';
            return;
        }
        if (top != 0){
            hole = left + 1 + (rand() % (right - left -1));
            maze[top][hole] = ' ';
            return;
        }
        if (bottom != SIZE-1){
            hole = left + 1 + (rand() % (right - left -1));
            maze[bottom][hole] = ' ';
            return;
        }
    }
    
}

void min_distances(char maze[SIZE][SIZE], int dist[SIZE][SIZE][SIZE][SIZE])
{
    int ix,iy,jx,jy,kx,ky;
    for(int ix = 0; ix < SIZE; ix++){
        for(int iy = 0; iy < SIZE; iy++){
            for(int jx = 0; jx<SIZE; jx++){
                for(int jy = 0; jy<SIZE; jy++){
                    dist[ix][iy][jx][jy] = INF;
                }
            }
            dist[ix][iy][ix][iy] = 0;
            if(maze[ix][iy] != 'X'){
                if(maze[std::min(ix + 1, SIZE-1)][iy] != 'X')
                    dist[ix][iy][std::min(ix + 1, SIZE-1)][iy] = 1;
                if(maze[std::max(ix - 1, 0)][iy] != 'X')
                    dist[ix][iy][std::max(ix - 1, 0)][iy] = 1;
                if(maze[ix][std::min(iy + 1, SIZE-1)] != 'X')
                    dist[ix][iy][ix][std::min(iy + 1, SIZE-1)] = 1;
                if(maze[ix][std::max(iy - 1, 0)] != 'X')
                    dist[ix][iy][ix][std::max(iy - 1, 0)] = 1;
            }
        }
    }

    for(kx = 0; kx < SIZE; kx++){
        for(ky = 0; ky < SIZE; ky++){
            if(maze[kx][ky] == ' '){
                for(ix = 0; ix < SIZE; ix++){
                    for(iy = 0; iy < SIZE; iy++){
                        if(maze[ix][iy] == ' ' && dist[ix][iy][kx][ky] < INF){
                            for(jx = 0; jx < SIZE; jx++){
                                for(jy = 0; jy < SIZE; jy++){
                                    if(maze[jx][jy] == ' '){
                                        if(dist[ix][iy][kx][ky] + dist[kx][ky][jx][jy] < dist[ix][iy][jx][jy]){
                                            dist[ix][iy][jx][jy] = dist[ix][iy][kx][ky] + dist[kx][ky][jx][jy];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
int main()
{
    srand(time(0));
    init_maze();
    partition(0, SIZE - 1, 0, SIZE - 1);
    print_maze();
    return 0;
}
*/