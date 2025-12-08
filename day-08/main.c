#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LIST_CAP 1024

int compare_sizes(const void *a, const void *b)
{
    size_t s1 = *(size_t *)a;
    size_t s2 = *(size_t *)b;
    return s2 - s1;
}

typedef struct
{
    size_t parent[MAX_LIST_CAP];
} UF;

void uf_init(UF *uf, size_t len)
{
    assert(len < MAX_LIST_CAP);
    for (size_t i = 0; i < len; i++)
    {
        uf->parent[i] = i;
    }
}

size_t uf_find(UF *uf, size_t i)
{
    if (uf->parent[i] == i)
        return i;
    return uf->parent[i] = uf_find(uf, uf->parent[i]);
}

int uf_union(UF *uf, size_t i, size_t j)
{
    i = uf_find(uf, i);
    j = uf_find(uf, j);
    if (i == j)
        return 0;
    uf->parent[j] = i;
    return 1;
}

typedef struct
{
    long x;
    long y;
    long z;
} Coord;

double coord_distance(Coord c1, Coord c2)
{
    return sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y) + (c1.z - c2.z) * (c1.z - c2.z));
}

typedef struct
{
    Coord data[MAX_LIST_CAP];
    size_t len;
} CoordList;

void coord_list_append(CoordList *cl, Coord c)
{
    assert(cl->len < MAX_LIST_CAP);
    cl->data[cl->len++] = c;
}

typedef struct
{
    size_t c1_idx;
    size_t c2_idx;
    double dist;
} CoordPair;

int compare_coord_pairs(const void *a, const void *b)
{
    CoordPair cp1 = *(CoordPair *)a;
    CoordPair cp2 = *(CoordPair *)b;
    return (cp1.dist > cp2.dist) - (cp1.dist < cp2.dist);
}

typedef struct
{
    CoordPair *data;
    size_t cap;
    size_t len;
} CoordPairList;

void coord_pair_init(CoordPairList *cpl, size_t cap)
{
    cpl->data = malloc(sizeof(CoordPair) * cap);
    cpl->cap = cap;
    cpl->len = 0;
}

void coord_pair_free(CoordPairList *cpl)
{
    free(cpl->data);
}

void coord_pair_list_append(CoordPairList *cpl, CoordPair cp)
{
    assert(cpl->len < cpl->cap);
    cpl->data[cpl->len++] = cp;
}

long calc_top3_largest_circuits(CoordList *cl, size_t k)
{
    size_t n = cl->len;
    size_t total = n * (n - 1) / 2;

    CoordPairList cpl;
    coord_pair_init(&cpl, total);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            CoordPair cp = {i, j, coord_distance(cl->data[i], cl->data[j])};
            coord_pair_list_append(&cpl, cp);
        }
    }

    qsort(cpl.data, cpl.len, sizeof(CoordPair), compare_coord_pairs);

    UF uf;
    uf_init(&uf, n);

    assert(k < total);
    for (size_t i = 0; i < k; i++)
    {
        size_t c1_idx = cpl.data[i].c1_idx;
        size_t c2_idx = cpl.data[i].c2_idx;
        uf_union(&uf, c1_idx, c2_idx);
    }

    coord_pair_free(&cpl);

    size_t counter[MAX_LIST_CAP] = {0};
    for (size_t i = 0; i < n; i++)
    {
        counter[uf_find(&uf, i)]++;
    }

    qsort(counter, n, sizeof(size_t), compare_sizes);

    return 1L * counter[0] * counter[1] * counter[2];
}

long calc_last2_connections(CoordList *cl)
{
    size_t n = cl->len;
    size_t total = n * (n - 1) / 2;

    CoordPairList cpl;
    coord_pair_init(&cpl, total);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            CoordPair cp = {i, j, coord_distance(cl->data[i], cl->data[j])};
            coord_pair_list_append(&cpl, cp);
        }
    }

    qsort(cpl.data, cpl.len, sizeof(CoordPair), compare_coord_pairs);

    long res = 0;

    UF uf;
    uf_init(&uf, n);

    for (size_t i = 0; i < cpl.len; i++)
    {
        if (uf_union(&uf, cpl.data[i].c1_idx, cpl.data[i].c2_idx))
        {
            res = cl->data[cpl.data[i].c1_idx].x * cl->data[cpl.data[i].c2_idx].x;
        }
    }

    coord_pair_free(&cpl);

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

    CoordList cl = {0};

    char buf[64];
    while (fgets(buf, sizeof(buf), f))
    {
        Coord c = {0};
        sscanf(buf, "%ld,%ld,%ld", &c.x, &c.y, &c.z);
        coord_list_append(&cl, c);
    }

    fclose(f);

    long part1 = calc_top3_largest_circuits(&cl, 1000);
    long part2 = calc_last2_connections(&cl);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
