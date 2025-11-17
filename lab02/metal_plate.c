/*******************************************************************************
 *
 * Lab02 (C99): metal_plate.c -- Calculates the temperature of a metal plate.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/******************************************************************************/

#define N 10
#define M 20

#define UP_SIDE 2.0
#define RIGHT_SIDE -5.0
#define BOTTOM_SIDE 3.0
#define LEFT_SIDE 4.0
#define INSIDE 1.0

#define THRESHOLD 0.000001

/******************************************************************************/

void init_array(double [N][M], double, double, double, double, double);
void _init_array_ln(double [N][M], int, double, double, double);

void print_array(const double [N][M]);
void print_norm_array(const int [N][M]);
void print_histogram(const int [10]);

double calc_array_temp(const double [N][M], double [N][M]);
double _sum_delta_temp(const double [N][M], const double [N][M]);

void calc_norm_array(const double [N][M], int [N][M]);
void _min_max_array(const double [N][M], double *, double *);
void calc_histogram(const int [N][M], int [10]);

/******************************************************************************/

int main(void)
{
    static double plate_temp[N][M]; /* The plate's temperature. */
    static double tmp_arr[N][M]; /* Auxiliary array for calculations. */
    static int norm_plate[N][M]; /* Normalized plate's temperature. */
    static int histogram[10]; /* Histogram's values. */

    double delta_temp; /* The difference of cells between two time steps. */
    int seconds = 0; /* Current time step. */

    init_array(plate_temp, UP_SIDE, RIGHT_SIDE, BOTTOM_SIDE, LEFT_SIDE, INSIDE);
    init_array(tmp_arr, UP_SIDE, RIGHT_SIDE, BOTTOM_SIDE, LEFT_SIDE, INSIDE);

    do {
        /* calculates the temperature for each step. */
        delta_temp = calc_array_temp(plate_temp, tmp_arr);
        /* copies the new temperature from the second array. */
        memcpy(plate_temp, tmp_arr, sizeof(double) * N * M);
        seconds++;
    } while (delta_temp > THRESHOLD);

    printf("Permanent state after %d (s) for threshold: %f\n\n",
        seconds, THRESHOLD);
    print_array(plate_temp);

    calc_norm_array(plate_temp, norm_plate);
    print_norm_array(norm_plate);

    calc_histogram(norm_plate, histogram);
    print_histogram(histogram);

    return EXIT_SUCCESS;
}

/******************************************************************************\
 * Functions used for array initialization.                                   *
\******************************************************************************/

/**
 * @brief Sets the initial values of the plate.
 *
 * @param array[N][M] The array with the initial temperature.
 * @param up The temperature of the upper side.
 * @param right The temperature of the right side.
 * @param bottom The temperature of the bottom side.
 * @param left The temperature of the left side.
 * @param in The temperature inside the plate.
 */
void init_array(double array[N][M],
    double up, double right, double bottom, double left, double in)
{
    for (int i = 0; i < N; ++i) { /* N is the number of rows. */
        if (i == 0) { /* The first i is the upper side. */
            _init_array_ln(array, i, (up + left) / 2, up, (up + right) / 2);
        } else if (i == N - 1) { /* The last i is the bottom side. */
            _init_array_ln(array, i,
                (bottom + left) / 2, bottom, (bottom + right) / 2);
        } else { /* The rest rows inside the plate have the same values. */
            _init_array_ln(array, i, left, in, right);
        }
    }

    return ;
}

/**
 * @brief Initializes the temperature of a certain row (line).
 *
 * @param array[N][M] The array with the initial temperature.
 * @param row_index The index of the row (line).
 * @param first The value of the left side cell.
 * @param inner The value of the inner cells.
 * @param last The value of the right side cell.
 */
void _init_array_ln(double array[N][M], int row_index,
    double first, double inner, double last)
{
    for (int i = 0; i < M; ++i) { /* M is the number of columns. */
        if (i == 0) {
            array[row_index][i] = first;
        } else if (i == M - 1) {
            array[row_index][i] = last;
        } else {
            array[row_index][i] = inner;
        }
    }

    return ;
}

/******************************************************************************\
 * Functions for printing data.                                               *
\******************************************************************************/

/**
 * @brief Prints an array of doubles.
 *
 * @param array[N][M] The array to be printed.
 */
void print_array(const double array[N][M])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            printf("%5.2f ", array[i][j]);
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);

    return ;
}

/**
 * @brief Prints an array of integers.
 *
 * @param norm_array[N][M] The array to be printed.
 */
void print_norm_array(const int norm_array[N][M])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            printf("%d ", norm_array[i][j]);
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);

    return ;
}

/**
 * @brief Prints a horizontal histogram.
 *
 * @param histogram[10] The histogram to be printed.
 */
void print_histogram(const int histogram[10])
{
    for (int i = 0; i < 10; ++i) {
        printf("%d: ",i);
        for (int j = 0; j < histogram[i]; ++j) {
            putc('#', stdout);
        }
        putc('\n', stdout);
    }
    putc('\n', stdout);

    return ;
}

/******************************************************************************\
 * Functions used for calculating the temperature of the plate.               *
\******************************************************************************/

/**
 * @brief Calculates the temperature of the plate in the next second.
 *
 * @param src_array[N][M] The temperature at the current state.
 * @param dest_array[N][M] The calculated temperature is stored here.
 *
 * @returns The sum of the absolute difference of all cells between those
 * two states.
 */
double calc_array_temp(const double src_array[N][M],
    double dest_array[N][M])
{
    /*
     * For the purpose of this exercise it is assumed that
     * the temperature on edges remains the same.
     * It calculates the temperature only on the inside.
     */
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < M - 1; ++j) {
            dest_array[i][j] = 0.1 * (src_array[i - 1][j - 1] +
                src_array[i - 1][j] +
                src_array[i - 1][j+1] +
                src_array[i][j - 1] +
                2 * src_array[i][j] +
                src_array[i][j + 1] +
                src_array[i + 1][j - 1] +
                src_array[i + 1][j] +
                src_array[i + 1][j + 1]
            );
        }
    }

    return _sum_delta_temp(src_array, dest_array);
}

/**
 * @brief Calculates the sum of the absolute difference for each cell,
 * between two arrays.
 *
 * @param arr_first[N][M] The first array.
 * @param arr_second[N][M] The second array.
 *
 * @returns The sum of the absolute difference.
 */
double _sum_delta_temp(const double arr_first[N][M],
    const double arr_second[N][M])
{
    double delta_temp = 0.0;

    /* Only the temperature on the inside of the plate changes. */
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < M - 1; ++j) {
            delta_temp += fabs(arr_first[i][j] - arr_second[i][j]);
        }
    }

    return delta_temp;
}

/******************************************************************************\
 * Functions used for calculating normalized plate temp and histogram.        *
\******************************************************************************/

/**
 * @brief Normalizes an array.
 *
 * @param array[N][M] The array for normalization.
 * @param norm_array[N][M] The normalized array is stored here.
 */
void calc_norm_array(const double array[N][M], int norm_array[N][M])
{
    double min, max;
    double width, step;
    double limit;
    int norm_value;

    /*
     * In order to calculate the width and the step,
     * it first finds the min and max value of the array.
     */
    _min_max_array(array, &min, &max);
    width = max - min;
    step = width / 10;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            /*
             * Limit is the lower value each cell can take
             * if the normalized value is equal to the current value.
             */
            limit = max - step;
            norm_value = 9;
            while (1) {
                /* The first condition is never valid for the min value. */
                if (array[i][j] > limit || norm_value == 0) {
                    norm_array[i][j] = norm_value;
                    break;
                }
                /* In each iteration decreases value and limit. */
                norm_value--;
                limit -= step;
            }
        }
    }

    return ;
}

/**
 * @brief Finds the min and the max of an array.
 *
 * @param array[N][M] The array where searches min and max values.
 * @param min A pointer to the array's minimum value.
 * @param max A pointer to the array's maximum value.
 */
void _min_max_array(const double array[N][M], double *min, double *max)
{
    double cur_cell; /* current cell. */

    *min = *max = array[0][0];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cur_cell = array[i][j];
            *max = fmax(*max, cur_cell);
            *min = fmin(*min, cur_cell);
        }
    }

    return ;
}

/**
 * @brief Calculates the histogram of a normalized array.
 *
 * @param norm_array[N][M] An array of integers with values from 0 to 9.
 * @param histogram[10] An array with the number of times each value occurred.
 *
 * @note Make sure that `norm_array` has proper values before call.
 */
void calc_histogram(const int norm_array[N][M], int histogram[10])
{
    int index; /* index to a histogram's cell. */

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            /* norm value is the same with the index. */
            index = norm_array[i][j];
            histogram[index]++;
        }
    }

    return ;
}
