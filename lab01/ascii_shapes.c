/*******************************************************************************
 *
 * Lab01 (C99) : ascii_shapes.c -- Prints ascii shapes at the command line.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/

int get_user_choice(void);
int get_shape_size(void);
int _read_one_int(void);

void print_square(int);
void print_rhombus(int);
void print_right(int);
void print_isosceles(int);

int _check_index_pos(int, int);
char _set_outer_ch(int);

void _print_outer_line(char, int, int);
void _print_inner_line(char, char, int);
void _print_middle_line(char, char, char, int);

/******************************************************************************/

enum choice
{
    INVALID = -2,
    EXIT = -1,
    SQUARE = 1,
    RHOMBUS = 2,
    RIGHT_TRIANGLE = 3,
    ISOSCELES_TRIANGLE = 4
};

enum position
{
    FIRST,
    SECOND,
    BEFORE_MIDDLE,
    MIDDLE_EVEN,
    MIDDLE,
    AFTER_MIDDLE,
    LAST
};

#ifndef DIGIT /* if DIGIT is true, the output char changes */
#define DIGIT 0
#endif

/******************************************************************************/

int main(void)
{
    int user_choice = 0;
    int shape_size;

    while (user_choice != EXIT) {
        user_choice = get_user_choice();
        if (user_choice == EXIT) {
            break;
        }
        shape_size = get_shape_size();
        if (shape_size <= 0) { /* Can't print a nonpositive shape size. */
            user_choice = INVALID;
        }
        switch(user_choice)
        {
        case SQUARE:
            print_square(shape_size);
            break;
        case RHOMBUS:
            print_rhombus(shape_size);
            break;
        case RIGHT_TRIANGLE:
            print_right(shape_size);
            break;
        case ISOSCELES_TRIANGLE:
            print_isosceles(shape_size);
            break;
        default:
            puts("Select a valid number.");
            break;
        }
    }

    return EXIT_SUCCESS;
}

/******************************************************************************\
 *  Functions used for reading input.                                         *
\******************************************************************************/

/**
 * @brief Prints the available shape options, reads an integer using
 * `_read_one_int(void)`.
 *
 * @returns An integer of the selected shape.
 */
int get_user_choice(void)
{
    int choice;

    puts("Select shape (-1 for exit).");
    printf(
        "%d) square \t\t %d) rhombus, \n"
        "%d) right triangle \t %d) isosceles triangle\n",
        SQUARE, RHOMBUS, RIGHT_TRIANGLE, ISOSCELES_TRIANGLE);

    choice = _read_one_int();

    return choice;
}

/**
 * @brief Asks for the size of shape, reads an integer using
 * `_read_one_int(void)`.
 *
 * @returns An integer representing the size of the shape.
 *
 * @note It doesn't check if the integer is positive.
 */
int get_shape_size(void)
{
    int size;

    puts("Give the size of the selected shape");

    size = _read_one_int();

    return size;
}

/**
 * @brief Reads an integer from terminal, ends the program in case of invalid
 * input.
 *
 * @returns An integer.
 */
int _read_one_int(void)
{
    int num;

    if (scanf("%d", &num) != 1) {
        perror("Invalid input");
        exit(EXIT_FAILURE);
    }

    return num;
}

/******************************************************************************\
 * Functions for printing the different shapes.                               *
\******************************************************************************/

/**
 * @brief Prints a square of the given size.
 *
 * @param size The number of rows.
 */
void print_square(int size)
{
    /* Characters for printing the shape, applies for all shapes. */
    char ch; /* It's value depends on DIGIT. */
    const char inner_ch = '~';
    const char center_ch = '*';

    for (int i = 0; i < size; ++i) {
        ch = _set_outer_ch(i);
        switch (_check_index_pos(i, size))
        {
        case FIRST: /* First and last line are identical. */
        case LAST:
            _print_outer_line(ch, size, 1);
            break;
        case MIDDLE_EVEN: /* Fall through, */
        case MIDDLE: /* middle lines are also identical. */
            _print_middle_line(ch, inner_ch, center_ch, size);
            break;
        default: /* Every other line is an inner shape line. */
            _print_inner_line(ch, inner_ch, size);
            break;
        }
    }

    return ;
}

/**
 * @brief Prints a rhombus of the given size.
 *
 * @param size The number of rows.
 */
void print_rhombus(int size)
{
    char ch;
    const char inner_ch = '~';
    const char center_ch = '*';
    const char outer_ch = '_';

    /* Each line has two lengths. */
    /* The length of chars a line has inside the shape. */
    int len_in_shape = 1;
    /* The length of chars a line has outside the shape. */
    int len_out_shape = (size % 2 != 0) ? size / 2 : size / 2 - 1;

    for (int i = 0; i < size; ++i) {
        ch = _set_outer_ch(i);
        switch (_check_index_pos(i, size))
        {
        case LAST: /* First and last line are identical. */
        case FIRST: /* Length increases from first to second line. */
            _print_outer_line(outer_ch, len_out_shape--, 0);
            _print_outer_line(ch, len_in_shape, 1);
            len_in_shape += 2;
            break;
        case SECOND: /* It is before middle so fall through, */
        /* ln_out decreases and ln_in increases before middle. */
	case BEFORE_MIDDLE:
            _print_outer_line(outer_ch, len_out_shape--, 0);
            _print_inner_line(ch, inner_ch, len_in_shape);
            len_in_shape += 2;
            break;
	/* Prints another middle before the actual for even length. */
        case MIDDLE_EVEN:
            _print_middle_line(ch, inner_ch, center_ch, len_in_shape);
            break;
        case MIDDLE: /* Length decreases (opposite pattern above). */
            _print_middle_line(ch, inner_ch, center_ch, len_in_shape);
            len_in_shape -= 2;
            len_out_shape++;
            break;
	/* The rest lines are after the middle and before last line. */
        case AFTER_MIDDLE:
            _print_outer_line(outer_ch, len_out_shape++, 0);
            _print_inner_line(ch, inner_ch, len_in_shape);
            len_in_shape -= 2;
            break;
        }
    }

    return ;
}

/**
 * @brief Prints a right triangle of a given size.
 *
 * @param size The number of rows.
 */
void print_right(int size)
{
    char ch;
    const char inner_ch = '~';

    for (int i = 0; i < size; ++i) {
        ch = _set_outer_ch(i);
        switch (_check_index_pos(i, size))
        {
        case FIRST: /* It is outer line with length 1, */
        case SECOND: /* it is outer line because it has length 2,*/
        case LAST: /* it is also outer with length equal the number of rows. */
            _print_outer_line(ch, i + 1, 1);
            break;
        default: /* The rest of the lines are inner. */
            _print_inner_line(ch, inner_ch, i + 1);
            break;
        }
    }

    return ;
}

/**
 * @brief Prints a isosceles triangle of a given size.
 *
 * @param size The number of rows.
 */
void print_isosceles(int size)
{
    char ch;
    const char inner_ch = '-';
    const char outer_ch = ' ';

    /* The length of chars a line has inside the shape. */
    int len_in_shape = 1;
    /* The length of chars a line has outside the shape. */
    int len_out_shape = size - 1;

    for (int i = 0; i < size; ++i) {
        ch = _set_outer_ch(i);
        switch (_check_index_pos(i, size))
        {
        case FIRST: /* First line is outer with length size, falls through. */
        case LAST: /* Last line is also outer with bigger size. */
            _print_outer_line(outer_ch, len_out_shape--, 0);
            _print_outer_line(ch, len_in_shape, 1);
            break;
        default: /* All the other lines are inner and there is no middle */
            _print_outer_line(outer_ch, len_out_shape--, 0);
            _print_inner_line(ch, inner_ch, len_in_shape);
            break;
        }
        /* In every iteration, length inside the shape becomes bigger. */
        len_in_shape += 2;
    }

    return ;
}

/******************************************************************************/

/**
 * @brief Checks the position of an index with a certain length.
 *
 * @param index The number to check the position.
 * @param length The number indices can reach.
 */
int _check_index_pos(int index, int length)
{
    enum position pos;

    /* FIRST and LAST are needed for every shape. */
    if (index == 0) {
        pos = FIRST;
    /* FIRST and LAST are also used in every line. */
    } else if (index == length - 1) {
        pos = LAST;
    /* MIDDLE used in square and rhombus. */
    } else if (length / 2 == index) {
        pos = MIDDLE;
    /* MIDDLE_EVEN is a duplicated middle line for even sizes. */
    } else if (length % 2 == 0 && length / 2 == index + 1) {
        pos = MIDDLE_EVEN;
    /* SECOND is needed for the right triangle,
        as a result it is used in rhombus in order to
        work properly. Using default case avoids further usage. */
    } else if (index == 1) {
        pos = SECOND;
    /* AFTER_MIDDLE and BEFORE_MIDDLE are used on rhombus. */
    } else if (index > length / 2) {
        pos = AFTER_MIDDLE;
    } else {
        pos = BEFORE_MIDDLE;
    }

    return pos;
}

/**
 * @brief Sets the proper char to print shapes with.
 *
 * @param num The current line of print loop.
 *
 * @returns If DIGIT is true it returns the last digit of the current line
 * else the character `'+'`.
 */
char _set_outer_ch(int num)
{
    return (DIGIT) ? (num % 10 + '0') : '+';
}

/******************************************************************************\
 * Functions used for printing the different type of lines.                   *
\******************************************************************************/

/**
 * @brief Prints a line of a given char and length.
 *
 * @param ch The char it prints.
 * @param length The number of times the char is printed.
 * @param nl In case it is nonzero, prints a new line.
 */
void _print_outer_line(char ch, int length, int nl)
{
    for (int i = 0; i < length; ++i) {
        putc(ch, stdout);
    } if (nl) { /* The functions below always print new line. */
        putc('\n', stdout);
    }

    return ;
}

/**
 * @brief Prints a line of two given chars and length.
 *
 * @param ch_out The char on the edges of the line.
 * @param ch_in The char on the inside of the line.
 * @param length The number of times the char is printed.
 */
void _print_inner_line(char ch_out, char ch_in, int length)
{
    for (int i = 0; i < length; ++i) {
        switch (_check_index_pos(i, length))
        {
        case FIRST: /* First and last char are always the same, fall through. */
        case LAST:
            putc(ch_out, stdout);
            break;
        default: /* The rest chars are inside the line. */
            putc(ch_in, stdout);
            break;
        }
    }
    putc('\n', stdout);

    return ;
}

/**
 * @brief Prints a line of three given chars and a length.
 *
 * @param ch_out The char on the edges of the line.
 * @param ch_in The char on the inside of the line.
 * @param ch_m The char on the middle of the line.
 * @param length The number of times the char is printed.
 */
void _print_middle_line(char ch_out, char ch_in, char ch_m, int length)
{
    for (int i = 0; i < length; ++i) {
        switch (_check_index_pos(i, length))
        {
        case FIRST: /* First and last char are always the same, fall through. */
        case LAST:
            putc(ch_out, stdout);
            break;
        case MIDDLE_EVEN: /* Middle lines are the same, fall through. */
        case MIDDLE:
            putc(ch_m, stdout);
            break;
        default: /* The rest chars are inside the line. */
            putc(ch_in, stdout);
            break;
        }
    }
    putc('\n', stdout);

    return ;
}
