#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 256
#define MAX_COLS 256

typedef struct
{
    int x, y;
} Pair;

Pair dirs[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int check(char grid[MAX_ROWS][MAX_ROWS], int start_x, int start_y, int rows, int cols)
{
    if (grid[start_x][start_y] == '.')
        return 0;

    int count = 0;

    for (size_t di = 0; di < 8; di++)
    {
        Pair dir = dirs[di];

        int nx = start_x + dir.x;
        int ny = start_y + dir.y;

        if (nx < 0 || nx >= rows || ny < 0 || ny >= cols)
            continue;

        if (grid[nx][ny] != '.')
        {
            count++;
            if (count >= 4)
                return 0;
        }
    }

    return 1;
}

void sweep(char grid[MAX_ROWS][MAX_ROWS], size_t rows, size_t cols)
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (grid[i][j] == 'X')
                grid[i][j] = '.';
        }
    }
}

int main(void)
{
    FILE *f = fopen("./input.txt", "r");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open the input\n");
        return 1;
    }

    char grid[MAX_ROWS][MAX_COLS];

    size_t rows = 0;
    while (fgets(grid[rows], sizeof(grid[rows]), f) != NULL)
        rows++;

    fclose(f);

    size_t cols = strlen(grid[0]) - 1;

    int part1 = 0;
    int part2 = 0;

    int count;
    do
    {
        count = 0;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {

                if (check(grid, i, j, rows, cols))
                {
                    grid[i][j] = 'X';
                    count++;
                }
            }
        }

        sweep(grid, rows, cols);

        part2 += count;

        if (part1 == 0)
        {
            part1 += count;
        }

    } while (count != 0);

    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2);

    return 0;
}
