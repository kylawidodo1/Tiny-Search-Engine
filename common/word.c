/*
 * word.c     Kyla Widodo     April 24, 2023
 *
 * Module providing a function to normalize a word
 *
 */

#include <ctype.h>
#include <string.h>
#include "word.h"

/**************** word_normalize ****************/
char* word_normalize(char *word) {
    if (word == NULL) {
        return NULL;
    }

    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
    return word;
}