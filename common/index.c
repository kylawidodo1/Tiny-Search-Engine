/*
 * index.c     Kyla Widodo     April 24, 2023
 *
 * Module providing the data structure to represent the in-memory index, and functions to read and write index files
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "file.h"
#include "counters.h"
#include "mem.h"

typedef struct index {
    hashtable_t* hashtable;
} index_t;

/**************** Helper Functions ****************/
static void counters_delete_help(void* item);
static void save_item(void* arg, const char* key, void* item);
static counters_t* word_counters(index_t* index, const char* word);
static void load_item(void* arg, const char* word, int docID, int count);

/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new(int size) {
    if (size > 0) {
        index_t *index = mem_assert(malloc(sizeof(index_t)), "Error: malloc failed");
        index->hashtable = hashtable_new(size); //New hashtable
        if (index->hashtable == NULL) { //If null, error
            fprintf(stderr, "Error: index in invalid\n");
            free(index);
            return NULL;
        } else {
            return index; 
        }
    } else {
        fprintf(stderr, "Error: Size of hashtable is invalid\n");
        return NULL;
    }
}


/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t *index) {
    if (index != NULL) {
        hashtable_delete(index->hashtable, counters_delete_help);
        free(index);
    }
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t *index, const char *word, int docID) { 
    if (index == NULL || word == NULL || docID < 1) {
        return false; 
    }
 
    counters_t* counters = hashtable_find(index->hashtable, word); 
    if (counters == NULL) {
        counters = counters_new();
        if (counters == NULL) {
            return false;
        }
        hashtable_insert(index->hashtable, word, counters);
    } 

    counters_add(counters, docID);
    return true;
}

/**************** index_increment() ****************/
/* see index.h for description */
// Increment the count of a word in a given document
bool index_increment(index_t* index, const char* word, const int docID) {
    if (index == NULL || word == NULL || docID < 1) {
        return false;
    }

    counters_t* counters = word_counters(index, word); 
    if (counters == NULL) {
        return false;
    }

    counters_add(counters, docID);
    return true;
}

/**************** index_save() ****************/
/* see index.h for description */
void index_save(index_t* index, const char* indexFilename) {
    if (index == NULL || indexFilename == NULL) {
        return;
    }

    FILE* fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to write a file named indexFilename\n");
        return;
    }
    
    hashtable_iterate(index->hashtable, fp, save_item); 
    fclose(fp);
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(const char* indexFilename) {
    FILE* fp = fopen(indexFilename, "r"); //Opens file
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file\n");
        return NULL;
    }

    index_t* index = index_new(file_numLines(fp)); 
    if (index == NULL) {
        fprintf(stderr, "Error: Unable to create new index\n");
        fclose(fp);
        return NULL;
    }

    char* word = NULL;
    int docID, count;

    while ((word = file_readWord(fp)) != NULL) { //Reads a word and its (docID, count) pairs 
        while ((fscanf(fp, "%d %d", &docID, &count)) == 2) { //For each pair, call load_item helper func
            load_item(index, word, docID, count);
        }
        free(word);
    }
    fclose(fp);
    return index;
}

/**************** index_find() ****************/
/* see index.h for description */
void* index_find(index_t* index, const char* word){
    if (index == NULL || word == NULL) {
        return NULL;
    } 
    return hashtable_find(index-> hashtable, word); 
}

/**************** Helper Functions ****************/
// Get the counters for a given word
static counters_t* word_counters(index_t* index, const char* word) {
    if (index == NULL || word == NULL) {
        return NULL;
    }

    return hashtable_find(index->hashtable, word); // Find the index
}

// Delete the counters when deleting the index
static void counters_delete_help(void* item) {
    counters_t* counters = item;
    counters_delete(counters); // Delete the counters item
}

//Counters iterate helper
static void counters_iterate_helper(void* arg, const int key, const int count) {
    FILE* fp = arg;
    fprintf(fp, " %d %d", key, count); //Prints the index output
}

// Save an item (word and counters) in the index file
static void save_item(void* arg, const char* key, void* item) {
    FILE* fp = arg;
    counters_t* counters = item; 

    fprintf(fp, "%s ", key); // Print word
    counters_iterate(counters, fp, counters_iterate_helper); // Iterate counter
    fprintf(fp, "\n");
}

// Load an item (word and counters) from the index file
static void load_item(void* arg, const char* word, int docID, int count) {
    index_t* index = arg;

    counters_t* counters = word_counters(index, word); // Retrieve counters 
    if (counters == NULL) { // If no counter, create new counters
        counters = counters_new();
        hashtable_insert(index->hashtable, word, counters); //Inset into the index's hashtable
    }

    for (int i = 0; i < count; i++) { // Adds the docID to counters
        counters_add(counters, docID);
    }
}