#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f = fopen("./input.txt", "r");

    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open the file");
        return 1;
    }

    int pass1 = 0;
    int pass2 = 0;

    int prevDial = 0;
    int dial1 = 50;
    int dial2 = 50;

    char buf[64];
    while (fgets(buf, sizeof(buf), f) != NULL)
    {
        char *p = buf;
        char dir = *p;
        p++;

        int n = 0;
        while (isdigit(*p))
        {
            n = n * 10 + (*p - '0');
            p++;
        }

        int steps = n % 100;

        prevDial = dial2;

        if (dir == 'R')
        {
            dial1 = (dial1 + n) % 100;
            if (dial1 == 0)
            {
                pass1++;
            }
            dial2 += steps;
        }
        else
        {
            dial1 = (dial1 - n + 100) % 100;
            if (dial1 == 0)
            {
                pass1++;
            }
            dial2 -= steps;
        }

        pass2 += n / 100;

        if (dial2 == 0)
        {
            pass2++;
        }
        else if (dial2 > 99)
        {
            pass2++;
            dial2 -= 100;
        }
        else if (dial2 < 0)
        {
            if (prevDial != 0)
            {
                pass2++;
            }
            dial2 += 100;
        }
    }

    printf("Part 1: %d\n", pass1);
    printf("Part 2: %d\n", pass2);

    fclose(f);

    return 0;
}
