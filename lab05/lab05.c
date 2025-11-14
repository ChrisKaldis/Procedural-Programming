/*******************************************************************************
 *
 * Lab05: lab05.c -- Function pointers example.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myand(int, int);
int myor(int, int);
int mynand(int, int);
int mynor(int, int);
int myxor(int, int);

/* New type, a pointer to a function that takes two int and returns one int */
typedef int (*CallBack) (int, int);

typedef struct {
    char gate_name[5];
    CallBack f;
} Gate;

int report(Gate);

int main(void)
{
    CallBack myfunctions[] = {myand, myor, myxor, mynand, mynor};
    char names [][5] = {"and", "or", "xor", "nand", "nor"};
    int n = sizeof(myfunctions) / sizeof(myfunctions[0]);
    int i;

    Gate *gates = malloc(n * sizeof(Gate));

    for (i = 0; i < n; i++) {
        gates[i].f = myfunctions[i];
        strcpy(gates[i].gate_name, names[i]);
        report(gates[i]);
    }

    free(gates);

    return EXIT_SUCCESS;
}

int myand(int a, int b)
{
    return a * b;
}

int myor(int a, int b)
{
    return a + b > 0;
}

int mynand(int a, int b)
{
    return a + b < 2;
}

int mynor(int a, int b)
{
    return a + b < 1;
}

int myxor(int a, int b)
{
    return a ^ b;
}

int report(Gate gate)
{
    int i, j;

    printf("Truth table of logic gate: %s\n", gate.gate_name);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("|%d, %d -> %d |\n", i, j, gate.f(i, j));
        }
    }
    putc('\n', stdout);

    return 0;
}
