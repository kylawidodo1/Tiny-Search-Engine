/*
 * indexertest.c     Kyla Widodo     April 27, 2023
 *
 * Unit testing
 * A program indextest will serve as a unit test for the index module
 * it reads an index file into the internal index data structure, then writes the index out to a new index file
 *
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"

int main(const int argc, char** argv) {
    //Reads an index file into the internal index data structure
    if(argc!=3){
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        exit(1);
    } else {
        // Create and load from readFile into inverted-index data structure
        index_t* index = index_load(argv[1]);

        // Save index
        index_save(index, argv[2]);

        // Delete Index
        index_delete(index);
    }
    return 0;
}