#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    long start;
    long end;
} Range;

int compare_ranges(const void *a, const void *b)
{
    Range r1 = *(Range *)a;
    Range r2 = *(Range *)b;
    if (r1.start < r2.start)
        return -1;
    if (r1.start > r2.start)
        return 1;
    if (r1.end > r2.end)
        return -1;
    if (r2.end > r1.end)
        return 1;
    return 0;
}

#define MAX_LIST_CAP 256

typedef struct
{
    Range data[MAX_LIST_CAP];
    size_t len;
} List;

void list_append(List *lst, Range range)
{
    assert(lst->len < MAX_LIST_CAP);
    lst->data[lst->len++] = range;
}

void list_sort(List *lst)
{
    qsort(lst->data, lst->len, sizeof(Range), compare_ranges);
}

int list_contains(List *lst, long id)
{
    for (size_t i = 0; i < lst->len; ++i)
    {
        if (lst->data[i].start <= id && id <= lst->data[i].end)
        {
            return 1;
        }
    }
    return 0;
}

long list_count_ranges(List *lst)
{
    long count = 0;
    Range prev = {0, -1};
    for (size_t i = 0; i < lst->len; i++)
    {
        Range cur = lst->data[i];
        if (cur.start > prev.end)
        {
            count += prev.end - prev.start + 1;
            prev = cur;
        }
        else if (cur.end > prev.end)
        {
            prev.end = cur.end;
        }
    }
    count += prev.end - prev.start + 1;
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

    int part1 = 0;

    List lst = {0};

    int reading_ids = 0;
    char buf[128];
    while (fgets(buf, sizeof(buf), f))
    {
        if (buf[0] == '\n')
        {
            list_sort(&lst);
            reading_ids = 1;
            continue;
        }

        if (!reading_ids)
        {
            Range r;
            sscanf(buf, "%ld-%ld", &r.start, &r.end);
            list_append(&lst, r);
        }
        else
        {
            long id;
            sscanf(buf, "%ld", &id);
            if (list_contains(&lst, id))
                part1++;
        }
    }

    fclose(f);

    long part2 = list_count_ranges(&lst);

    printf("Part 1: %d\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
