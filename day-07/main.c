#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS_CAP 256
#define MAX_COLS_CAP 256

typedef struct
{
    char data[MAX_ROWS_CAP][MAX_COLS_CAP];
    size_t rows;
    size_t cols;
} Grid;

size_t grid_get_start_col(Grid *grid)
{
    for (size_t j = 0; j < grid->cols; j++)
    {
        if (grid->data[0][j] == 'S')
        {
            return j;
        }
    }
    return -1;
}

long grid_count_splits(Grid *grid)
{
    char curr[MAX_COLS_CAP] = {0};
    char next[MAX_COLS_CAP] = {0};

    size_t start_col = grid_get_start_col(grid);
    curr[start_col] = 1;

    long count = 0;
    for (size_t row = 1; row < grid->rows; row++)
    {
        memset(next, 0, grid->cols);
        for (size_t col = 0; col < grid->cols; col++)
        {
            if (!curr[col])
                continue;

            char cell = grid->data[row][col];
            if (cell == '^')
            {
                count++;

                if (col > 0)
                    next[col - 1] = 1;
                if (col + 1 < grid->cols)
                    next[col + 1] = 1;
            }
            else
            {
                next[col] = 1;
            }
        }
        memcpy(curr, next, grid->cols);
    }
    return count;
}

long grid_count_timelines(Grid *grid)
{
    long dp_curr[MAX_COLS_CAP] = {0};
    long dp_next[MAX_COLS_CAP] = {0};

    size_t start_col = grid_get_start_col(grid);
    dp_curr[start_col] = 1;

    for (size_t i = 1; i < grid->rows; i++)
    {
        memset(dp_next, 0, grid->cols * sizeof(long));
        for (size_t j = 0; j < grid->cols; j++)
        {
            if (grid->data[i][j] == '.')
            {
                dp_next[j] += dp_curr[j];
            }
            else if (grid->data[i][j] == '^')
            {
                if (j > 0)
                    dp_next[j - 1] += dp_curr[j];
                if (j + 1 < grid->cols)
                    dp_next[j + 1] += dp_curr[j];
            }
            else
            {
                assert(0 && "Unexpected grid cell");
            }
        }
        memcpy(dp_curr, dp_next, grid->cols * sizeof(long));
    }

    long count = 0;
    for (size_t j = 0; j < grid->cols; j++)
    {
        count += dp_curr[j];
    }
    return count;
}

int main(void)
{
    FILE *f = fopen("./input.txt", "r");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open the input\n");
        return 1;
    }

    Grid grid = {};

    char buf[256];
    while (fgets(buf, sizeof(buf), f))
    {
        grid.cols = strlen(buf) - 1;
        assert(grid.cols < MAX_COLS_CAP);
        assert(grid.rows < MAX_ROWS_CAP);
        memcpy(grid.data[grid.rows++], buf, grid.cols);
    }

    fclose(f);

    long part1 = grid_count_splits(&grid);
    long part2 = grid_count_timelines(&grid);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
