/*******************************************************************************
 *
 * Lab01: lab01.c -- Prints ascii square at command line.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void print_square(int);

void print_out(int, char);
void print_in(int, char, char);
void print_center(int, char, char, char);


int main(void)
{
    int size;

    puts("Give number of rows:");
    scanf("%d", &size);

    print_square(size);

    return EXIT_SUCCESS;
}

void print_square(int size)
{
    int i;

    for (i = 0; i < size; ++i) {
        if (i == 0 || i == size - 1) {
            print_out(size, 'q');
        } else if (
            (i == size / 2) ||
            ((size % 2 == 0) && (i == size / 2 - 1))
        ) {
            print_center(size, '*', '-', 'a');
        } else {
            print_in(size, '-', 'a');
        }
    }

    return ;
}

void print_out(int n, char ch)
{
    int i;

    for (i = 0; i < n; i++) {
        putc(ch, stdout);
    }
    putc('\n', stdout);

    return ;
}

void print_in(int n, char in, char out)
{
    int i;

    for (i = 0; i < n; ++i) {
        if (i == 0 || i == n - 1) {
            putc(out, stdout);
        } else {
            putc(in, stdout);
        }
    }
    putc('\n', stdout);

    return ;
}

void print_center(int n, char c, char in, char out)
{
    int i;

    for (i = 0; i < n; ++i) {
        if (i == 0 || i == n - 1) {
            putc(out, stdout);
        } else if (
            (i == n / 2) ||
            ((n % 2 == 0) && (i == n / 2 - 1))
        ) {
            putc(c, stdout);
        } else {
            putc(in, stdout);
        }
    }
    putc('\n', stdout);

    return ;
}
