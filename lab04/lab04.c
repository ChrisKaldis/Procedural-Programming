/*******************************************************************************
 *
 * Lab04: lab04.c -- Struct example with dynamic memory allocation.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARS 10

typedef struct text_t {
    char **t; /* text */
    int words; /* number of words text has. */
} Text_t;

typedef struct charact {
    char ch; /* char from a word */
    int sec; /* the second time char appeared */
} Char;

Text_t fread_text(char *);

void letters(char *name, Char chars[50]);
void report(Char chars[50]);


int main(void)
{
    Char chars[50];
    Text_t text = fread_text("lab04/test.txt");

    for (int i = 0; i < text.words; ++i) {
        letters(text.t[i], chars);
        printf("\n>%s\n", text.t[i]);
        report(chars);
    }

    return EXIT_SUCCESS;
}

Text_t fread_text(char *filepath) {
    FILE *fp;
    Text_t text;
    char *tmp_word = malloc(CHARS * sizeof(char));

    text.t = NULL;
    text.words = 0;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return text;
    }

    while (fscanf(fp, "%10s", tmp_word) == 1) {
        text.words++;

        text.t = realloc(text.t, text.words * sizeof(char *));
        if (text.t == NULL) {
            perror("realloc failed");
            break;
        }

        text.t[text.words - 1] = malloc((strlen(tmp_word) + 1) * sizeof(char));
        if (text.t[text.words - 1] == NULL) {
            perror("malloc failed");
            break;
        }

        strcpy(text.t[text.words - 1], tmp_word);
    }
    fclose(fp);
    free(tmp_word);

    return text;
}

void letters(char *name, Char chars[50])
{
    size_t i, j;

    memset(chars, 0, 50 * sizeof(Char));

    for (i = 0; name[i] != 0; i++) {
        chars[i].ch = name[i];
        for (j = i + 1; name[j] != 0; ++j) {

            if (name[j] == chars[i].ch) {
                chars[i].sec = j - i;
                break;
            }
        }
    }

    return ;
}

void report(Char chars[50])
{
    size_t i;

    for (i = 0; chars[i].ch != 0; ++i) {
        printf("%c: %d\n", chars[i].ch, chars[i].sec);
    }

    return ;
}
