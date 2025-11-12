/*******************************************************************************
 *
 * Lab03: lab03.c -- String usage strlen, strcpy, strcmp.
 *
 * Author: Christos Kaldis
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define W 5 /* Number of words. */
#define L 9 /* Length for every word. */

#define SIZE 64 /* Size of test buffer. */

void read_words(char [W][L + 1]);
void trim_vowels(char [W][L + 1], char [W][L + 1]);
int is_vowel(char, int);
void reverse_words(char [W][L + 1], char [W][L + 1]);
void append_word(char [W][L + 1]);
void print_words(char [W][L + 1]);


int main(void)
{
    static char words[W][L + 1]; /* An array of words. */
    static char trimed[W][L + 1]; /* An array of words without vowels. */
    static char reversed[W][L + 1]; /* Words reversed. */

    read_words(words);
    trim_vowels(words, trimed);
    reverse_words(trimed, reversed);

    print_words(words);
    print_words(trimed);
    print_words(reversed);
    
    append_word(reversed);
    print_words(reversed);

    return EXIT_SUCCESS;
}

void read_words(char words[W][L+1])
{
    const char *endword = "*End*";
    char test[SIZE];
    int word_i = 0;
    int word_len = 0;
    char ch;

    puts("Type up to 5 words, type *End* to exit.");
    do {
        fgets(test, SIZE, stdin);

        word_len = strlen(test) - 1;
        if (word_len <= L) {
            test[word_len] = '\0';
            if (strcmp(test, endword) == 0) {
                break;
            } else {
                strcpy(words[word_i++], test);
            }
        } else {
            printf("Word is too large %d (> %d).\n", word_len, L);
        }

    } while (word_i < W);

    return ;
}

void reverse_words(char src[W][L + 1], char dest[W][L + 1])
{
    int word_len;
    int r_word_i; /* reversed word index */

    for (int i = 0; i < W; ++i) {
        r_word_i = 0;
        word_len = strlen(src[i]) - 1;
        for (int j = word_len; j >= 0; --j) {
            dest[i][r_word_i++] = src[i][j];
        }
    }

    return ;
}

void print_words(char words[W][L + 1])
{

    for (int i = 0; strlen(words[i]); i++) {
        printf("%d) %s\n", i, words[i]);
    }
    putc('\n', stdout);

    return ;
}

void trim_vowels(char text[W][L + 1], char trim_text[W][L + 1])
{
    int word_len;
    int k; /* trimed word index */

    for (int i = 0; i < W; ++i) {
        word_len = strlen(text[i]);
        k = 0;
        for (int j = 0; j < word_len; ++j) {
            if (!is_vowel(text[i][j], j)) {
                trim_text[i][k++] = toupper(text[i][j]);
            }
        }
    }

    return ;
}

int is_vowel(char ch, int i)
{
    int c = tolower((int) ch);

    if (c == 'a' || c == 'e' || c == 'o' || c == 'u' ||
        (c == 'y' && i == 0)) {
        return 1;
    }

    return 0;
}

void append_word(char text[W][L + 1])
{
    int i, j;
    char new_word[SIZE];

    for (i = 0; strlen(text[i]); i++)
        ; /* count how many words are already inserted. */

    if (i < W - 1) {
        puts("Give another new word:");
        scanf("%s", new_word);
        
        puts("Select new word index:");
        scanf("%d", &j);

        if (strlen(new_word) < L + 1) {
            for (int k = i + 1; k > j; --k) {
                strcpy(text[k], text[k-1]);
            }
            strcpy(text[j], new_word);
        }
    }

    return ;
}
