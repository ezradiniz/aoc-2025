#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    long first;
    long last;
} range;

long parse_int(char **p)
{
    long n = 0;
    while (isdigit(**p))
    {
        n = n * 10 + (**p - '0');
        (*p)++;
    }
    return n;
}

size_t numlen(long n)
{
    if (n == 0)
        return 1;
    size_t len = 0;
    while (n > 0)
    {
        n /= 10;
        len++;
    }
    return len;
}

typedef struct
{
    size_t len;
    long digits[20];
} dlist;

dlist digits_of(long n)
{
    dlist lst = {};
    lst.len = numlen(n);
    for (size_t i = 0; i < lst.len; i++)
    {
        lst.digits[lst.len - i - 1] = n % 10;
        n /= 10;
    }
    return lst;
}

bool is_invalid(dlist lst, size_t size)
{
    if (lst.len % size != 0)
        return false;
    for (size_t i = 0, j = 0; i < lst.len; i++, j = (j + 1) % size)
    {
        if (lst.digits[i] != lst.digits[j])
            return false;
    }
    return true;
}

long sum_invalid(range r, bool check_all)
{
    long sum = 0;
    for (long id = r.first; id <= r.last; id++)
    {
        dlist lst = digits_of(id);

        bool invalid = false;

        if (check_all)
        {
            for (size_t size = 1; size < lst.len; size++)
            {
                if (is_invalid(lst, size))
                {
                    invalid = true;
                    break;
                }
            }
        }
        else
        {
            if (lst.len % 2 == 0)
                invalid = is_invalid(lst, lst.len / 2);
        }

        if (invalid)
            sum += id;
    }
    return sum;
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

    char buf[1024];
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        char *p = buf;

        while (*p != '\0' && *p != '\n')
        {
            range r = {};

            r.first = parse_int(&p);

            if (*p == ',' || *p == '-')
                p++;

            r.last = parse_int(&p);

            if (*p == ',' || *p == '-')
                p++;

            part1 += sum_invalid(r, false);
            part2 += sum_invalid(r, true);
        }
    }

    fclose(f);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
