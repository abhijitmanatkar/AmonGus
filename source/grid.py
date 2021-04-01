import random

SIZE = 10

def print_grid(grid):
    for i in range(SIZE):
        for j in range(SIZE):
            print(grid[i][j], end="")
        print("")

grid = [[' ' for i in range(SIZE)] for j in range(SIZE)]

grid[0] = ['X' for i in range(SIZE)]
grid[SIZE - 1] = ['X' for i in range(SIZE)] 
for i in range(SIZE):
    grid[i][0] = grid[i][SIZE-1] = 'X'


exit_pos = random.randint(1, SIZE-2)

grid[0][exit_pos] = ' '

vertical = random.randint(2, SIZE - 3)
while vertical == exit_pos:
    vertical = random.randint(2, SIZE - 3)

horizontal = random.randint(2, SIZE - 3)

grid[horizontal] = ['X' for i in range(SIZE)]
for i in range(SIZE):
    grid[i][vertical] = 'X'




print_grid(grid)