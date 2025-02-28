/* 
 * word.h
 *
 * Normalizes words
 * 
 * Kyla Widodo, 2023
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/**************** word_normalize ****************/
/*
 * Converts all characters in the given word to lowercase
 * If the input word is NULL, the function does nothing 
 */
char* word_normalize(char* word);

#endif // __WORD_H