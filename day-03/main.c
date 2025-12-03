#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 128

typedef struct
{
    size_t count;
    int data[MAX_STACK_SIZE];
} Stack;

void stack_push(Stack *s, int val)
{
    assert(s->count < MAX_STACK_SIZE);
    s->data[s->count++] = val;
}

int stack_top(Stack *s)
{
    assert(s->count > 0);
    return s->data[s->count - 1];
}

int stack_pop(Stack *s)
{
    assert(s->count > 0);
    return s->data[--s->count];
}

void stack_clear(Stack *s)
{
    s->count = 0;
}

long stack_tonum(Stack *s, size_t len)
{
    assert(s->count >= len);
    long res = 0L;
    for (size_t i = 0; i < len; i++)
    {
        res = res * 10 + s->data[i];
    }
    return res;
}

void push_monotonic(Stack *s, int val, size_t remaining, size_t len)
{
    while (s->count > 0 && s->count + remaining > len && stack_top(s) < val)
    {
        stack_pop(s);
    }
    if (s->count < len)
    {
        stack_push(s, val);
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

    long part1 = 0L;
    long part2 = 0L;

    Stack s1 = {};
    Stack s2 = {};

    char buf[128];
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        size_t len = strlen(buf);

        int i = 0;
        for (char *p = buf; *p != '\0' && *p != '\n'; p++, i++)
        {
            int val = *p - '0';
            push_monotonic(&s1, val, len - i - 1, 2);
            push_monotonic(&s2, val, len - i - 1, 12);
        }

        part1 += stack_tonum(&s1, 2);
        part2 += stack_tonum(&s2, 12);

        stack_clear(&s1);
        stack_clear(&s2);
    }

    fclose(f);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}
