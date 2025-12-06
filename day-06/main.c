#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW_CAP 10
#define MAX_COL_CAP 1024
#define MAX_DIG_CAP 8

void apply_op(long *out, long in, char op)
{
    if (op == '+')
    {
        *out = *out + in;
    }
    else
    {
        *out = *out * in;
    }
}

typedef struct
{
    size_t col_start;
    size_t len;
    int val;
    char digits[MAX_DIG_CAP];
} Cell;

typedef struct
{
    Cell grid[MAX_ROW_CAP][MAX_COL_CAP];
    size_t rows;
} Worksheet;

void ws_parse_row(Worksheet *ws, char *row)
{
    assert(ws->rows < MAX_ROW_CAP);
    Cell *cells = ws->grid[ws->rows++];
    size_t cols = 0;
    char *p = row;
    while (*p != '\0' && *p != '\n')
    {
        Cell cell = {};
        while (*p == ' ')
            p++;
        cell.col_start = p - row;
        while (isdigit(*p))
        {
            assert(cell.len < MAX_DIG_CAP);
            cell.val = cell.val * 10 + (*p - '0');
            cell.digits[cell.len] = *p;
            cell.len++;
            p++;
        }
        cell.digits[cell.len] = '\0';
        assert(cols < MAX_COL_CAP);
        cells[cols++] = cell;
    }
}

long ws_calc_col(Worksheet *ws, size_t col, char op)
{
    long res = op == '+' ? 0 : 1;
    for (size_t row = 0; row < ws->rows; row++)
    {
        apply_op(&res, ws->grid[row][col].val, op);
    }
    return res;
}

long ws_calc_right_to_left(Worksheet *ws, size_t col, size_t op_col, char op)
{
    long res = (op == '+') ? 0 : 1;
    for (size_t pos = 0;; pos++)
    {
        int num = 0;

        for (size_t r = 0; r < ws->rows; r++)
        {
            Cell *c = &ws->grid[r][col];
            if (op_col >= c->col_start && op_col < c->col_start + c->len)
            {
                if (pos < c->len)
                    num = num * 10 + (c->digits[pos] - '0');
            }
            else
            {
                size_t abscol = op_col + pos;
                if (abscol >= c->col_start && abscol < c->col_start + c->len)
                    num = num * 10 + (c->digits[abscol - c->col_start] - '0');
            }
        }

        if (num == 0)
            break;

        apply_op(&res, num, op);
    }
    return res;
}

int main(void)
{
    FILE *f = fopen("./input.txt", "r");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open the input\n");
        return 1;
    }

    long part1 = 0;
    long part2 = 0;

    Worksheet ws = {};

    char buf[4096];
    while (fgets(buf, sizeof(buf), f))
    {
        if (!(buf[0] == '+' || buf[0] == '*'))
        {
            ws_parse_row(&ws, buf);
        }
        else
        {
            size_t col = 0;
            for (char *p = buf; *p && *p != '\n'; p++)
            {
                if (*p == '+' || *p == '*')
                {
                    part1 += ws_calc_col(&ws, col, *p);
                    part2 += ws_calc_right_to_left(&ws, col, p - buf, *p);
                    col++;
                }
            }
        }
    }

    fclose(f);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
