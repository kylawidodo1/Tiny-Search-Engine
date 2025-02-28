/* 
 * index.h
 *
 * Module providing the data structure to represent the in-memory index, and functions to read and write index files
 *  
 * Kyla Widodo, 2023
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "file.h"
#include "counters.h"
#include "mem.h"

#ifndef __INDEX_H
#define __INDEX_H

typedef struct index index_t;

/**************** index_new ****************/
/* 
 * Creates a new index with the given size
 * Returns a pointer to the new index, or NULL if there's an error
 */
index_t* index_new(int size);

/**************** index_delete ****************/
/* 
 * Deletes the index and frees all memory associated with it
 */
void index_delete(index_t* index);

/**************** index_insert ****************/
/* 
 * Inserts a word and its corresponding document ID into the index 
 * Returns true if the insertion was successful, false otherwise
 */
bool index_insert(index_t *index, const char *word, int docID);


/**************** index_increment ****************/
/* 
 * Increment the count of a word in a given document
 * Returns true if the count was incremented successfully, false otherwise
 */
bool index_increment(index_t* index, const char* word, const int docID);

/**************** index_save ****************/
/* 
 * Saves the index to a file with the specified name
 */
void index_save(index_t* index, const char* indexFilename) ;

/**************** index_load ****************/
/* 
 * Loads an index from a file with the specified name.
 * Returns a pointer to the loaded index, or NULL if there's an error
 */
index_t* index_load(const char* indexFilename); 

/**************** index_find ****************/
/* 
 * Finds the counters associated with the given word in the index
 * Returns a pointer to the counters, or NULL if the word isn't found in the index
 */
void* index_find(index_t* index, const char* word);

#endif // __INDEX_H
