/*******************************************************************************
 *
 * Lab02: lab02.c -- Array manipulation example.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define N 10
#define M 5

int load(double [N][M]);
int print(double [N][M]);

double compute_prod_sum(double [N][M], double *);
int print_prod_sum(double, double);

int transpose(double [N][M], double [M][N]);
int print_transposed(double [M][N]);


int main(void)
{
    static double data[N][M];
    static double transposed[M][N];
    double prod, sum;

    load(data);
    print(data);

    prod = compute_prod_sum(data, &sum);
    print_prod_sum(prod, sum);

    transpose(data, transposed);
    print_transposed(transposed);

    return EXIT_SUCCESS;
}

int load(double data[N][M])
{
    int i, j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            data[i][j] = ((double) i + j + N) / N;
        }
    }

    return EXIT_SUCCESS;
}

int print(double data[N][M])
{
    int i, j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            printf(" %.1f", data[i][j]);
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);

    return EXIT_SUCCESS;
}

double compute_prod_sum(double data[N][M], double *sum)
{
    int i, j;
    double prod = 1.0;

    *sum = 0.0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            prod *= data[i][j];
            *sum += data[i][j];
        }
    }

    return prod;
}

int print_prod_sum(double prod, double sum)
{
    printf("\nThe product is: %g and the sum is: %g\n", prod, sum);

    return EXIT_SUCCESS;
}

int transpose(double data[N][M], double data_t[M][N])
{
    int i, j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            data_t[j][i] = data[i][j];
        }
    }
}

int print_transposed(double data_transposed[M][N])
{
    int i, j;

    for (i = 0; i < M; ++i) {
        for (j = 0; j < N; ++j) {
            printf(" %.1f", data_transposed[i][j]);
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);

    return EXIT_SUCCESS;
}
