/*******************************************************************************
 *
 * Lab05 (ANSI C): logic_gates.c -- Prints the truth table of logic gates.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS 4

/******************************************************************************/

typedef struct data {
    int value;
    struct data * next;
} Data;

typedef Data *DataList;

typedef int (* CallBack) (DataList);

typedef struct gate {
    char gate_name[5];
    CallBack f;
} Gate;

/******************************************************************************/

int and_lst(DataList);
int or_lst(DataList);
int nand_lst(DataList);
int nor_lst(DataList);
int xor_lst(DataList);

Data *create_data(int value);
void append_data(DataList *lst_ptr, Data *new_ptr);
void print_data(DataList *lst_ptr);
void free_data(DataList *lst_ptr);

void report(Gate gate, int digits);

/******************************************************************************/

int main(void)
{
    CallBack myfunctions[] = {and_lst, or_lst, xor_lst, nand_lst, nor_lst};
    char names [][5] = {"and", "or", "xor", "nand", "nor"};
    int n = sizeof(myfunctions) / sizeof(myfunctions[0]);
    int i;

    Gate *gates = malloc(n * sizeof(Gate));

    if (gates == NULL) {
        perror("Memory allocation failed.");

        return EXIT_FAILURE;
    }

    for (i = 0; i < n; i++) {
        gates[i].f = myfunctions[i];
        strcpy(gates[i].gate_name, names[i]);
        report(gates[i], DIGITS);
    }

    free(gates);

    return EXIT_SUCCESS;
}

/******************************************************************************\
 * My implementation of logic gates.                                          *
\******************************************************************************/

/**
 * @brief Applies logic operator AND to a linked list.
 * 
 * @param x A `DataList` with gate's inputs.
 * 
 * @returns 1 if all the inputs are true otherwise 0.
 * 
 * @note It doesn't check if the input is valid.
 */
int and_lst(DataList x)
{
    /*
     * Looks the value of each data, if it is false then it returns 0, if 
     * value is true it continues to the next node, if it is at the end then it
     * returns 1 (every value was true).
     */
    if (x == NULL) {
        return 1;
    } else if (x->value == 0) {
        return 0;
    }

    return and_lst(x->next);
}

/**
 * @brief Applies logic operator OR to a linked list.
 * 
 * @param x A `DataList` with gate's inputs.
 * 
 * @returns 1 if at least one of the inputs are true otherwise 0.
 * 
 * @note It doesn't check if the input is valid.
 */
int or_lst(DataList x)
{
    /*
     * Looks the value of each data, if it is true then it returns 1, if it is 
     * false it continues to the next node, if it is at the end then it
     * returns 0 (every value was false).
     */
    if (x == NULL) {
        return 0;
    } else if (x->value == 1) {
        return 1;
    }

    return or_lst(x->next);
}

/**
 * @brief Applies logic operator NAND to a linked list.
 * 
 * @param x A `DataList` with gate's inputs.
 * 
 * @returns 1 if all the inputs are false otherwise 1.
 * 
 * @note Uses `myandlst(Datalist)`, it doesn't check if the input is valid.
 */
int nand_lst(DataList x)
{
    return !and_lst(x);
}

/**
 * @brief Applies logic operator NOR to a linked list.
 * 
 * @param x A `DataList` with gate's inputs.
 * 
 * @returns 1 if all the inputs are false otherwise 0.
 * 
 * @note Uses `myorlst(Datalist)`, it doesn't check if the input is valid.
 */
int nor_lst(DataList x)
{
    return !or_lst(x);
}

/**
 * @brief Applies logic operator NAND to a linked list.
 * 
 * @param x A `DataList` with gate's inputs.
 * 
 * @returns 1 if the number of true inputs is odd otherwise 0.
 * 
 * @note It doesn't check if the input is valid.
 */
int xor_lst(DataList x)
{
    if (x->next == NULL) {
        return x->value;
    } else {
        return x->value ^ xor_lst(x->next);
    }
}

/******************************************************************************\
 * Basic manipulation of Data struct.                                         *
\******************************************************************************/

/**
 * @brief Allocates memory for a new value in a `Data` struct.
 * 
 * @param value An integer that stores 0 or 1.
 * 
 * @returns A `Data` struct with the new value.
 * 
 * @note Doesn't check if the value is actually binary.
 */
Data *create_data(int value)
{
    Data *data_ptr;
    data_ptr = (Data *) malloc(sizeof(Data));

    if (data_ptr == NULL) {
        perror("Memory allocation failed in create_data.");
        exit(EXIT_FAILURE);
    }

    data_ptr -> value = value;
    data_ptr -> next = NULL;

    return data_ptr;
}

/**
 * @brief Adds a node in a linked list.
 * 
 * @param lst_ptr A pointer to the `DataList`.
 * @param new_ptr A pointer to the `Data`.
 */
void append_data(DataList *lst_ptr, Data *new_ptr)
{
    if (*lst_ptr == NULL) {
        *lst_ptr = new_ptr;

        return ;
    }

    append_data(&((*lst_ptr)->next), new_ptr);

    return ;
}

/**
 * @brief Deallocates memory of a linked list.
 * 
 * @param lst_ptr A pointer to the `DataList`.
 */
void free_data(DataList *lst_ptr)
{
    DataList current = *lst_ptr;
    DataList next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    *lst_ptr = NULL;

    return ;
}

/**
 * @brief Prints a linked list's values.
 * 
 * @param lst_ptr A pointer to the `DataList`.
 */
void print_data(DataList *lst_ptr)
{
    if (*lst_ptr == NULL) {
        printf(" |");

        return ;
    }

    printf("| %d",(*lst_ptr)->value);
    print_data(&(*lst_ptr)->next);

    return ;
}

/******************************************************************************/

/**
 * @brief Prints the truth table of a logic gate for a given number of digits.
 * 
 * @param gate The `Gate` truth table is printed.
 * @param digits The number of binary digits (number of inputs). 
 */
void report(Gate gate, int digits)
{
    DataList data_lst = NULL;
    /* The value of 2^(digits), used as an upper bound in for loop. */
    int max_bin = 1 << digits;
    int bin, cur_bin, i;

    printf("<%s> truth table for %d digits.\n", gate.gate_name, digits);
    for (bin = 0; bin < max_bin; bin++) {
        /* The value of input. */
        cur_bin = bin;
        for (i = 0; i < digits; i++) {
            /* adds the last digit, */
            append_data(&data_lst, create_data(cur_bin % 2));
            /* shifts the bits in order to get the next bit. */
            cur_bin = cur_bin >> 1;
        }

        /* First it prints the input, */
        print_data(&data_lst);
        /* then calculates gate's output. */
        printf("| %d |\n", gate.f(data_lst));
        /* Deallocates memory. */
        free_data(&data_lst);
    }
    putc('\n', stdout);

    return ;
}
