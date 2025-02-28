/*
 * indexer.c     Kyla Widodo     April 27, 2023
 *
 * The indexer program reads the webpages from the given pageDirectory and builds an index, which is then saved to the given indexFilename.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"


/**************** Function Prototypes ****************/
int main(const int argc, char* argv[]);
static index_t* indexBuild(char* pageDirectory, const char* indexFilename);
static void indexPage(webpage_t* page, int docID, index_t* index);

/**************** main() ****************/
/* main parses arguments and intializes other modules
 *
 */
int main(const int argc, char* argv[]){
    //Parse command line 
    if (argc != 3){ //Check # of arguments
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n"); //Print usage
        return 1; 
    } 
    //Assign arguments
    char *pageDirectory = argv[1];
    char *indexFilename = argv[2];

    //Error statements
    if (!pagedir_validate(pageDirectory)) { // If not a pathname for a directory produce by crawler
        fprintf(stderr, "Error: pageDirectory doesn't contain valid crawler output\n");
        return 2;
    }
    FILE* file;
    if ((file = fopen(indexFilename, "w")) == NULL) { //If cannot be written
        fprintf(stderr, "Error: Unable to write a file named indexFilename\n");
        return 3;
    }
    
    index_t* index = indexBuild(pageDirectory, indexFilename); // Call indexBuild, which calls on indexPage
    
    //Write to file
    index_save(index, indexFilename); // Use hashtable_iterate and save item
    index_delete(index); //Delete and close index
    return 0; 
}
    

/**************** indexBuild() ****************/
/* Builds an in-memory index from webpage files it finds in the pageDirectory
 * Creates a new 'index' object and loop over document ID, pass webpage and docID to indexPage
 */

static index_t* indexBuild(char* pageDirectory, const char* indexFilename) {
    const int size = 200; //Number of hashtable slots
    index_t* index = index_new(size); // Create new index hashtable
    if (index ==NULL) {
        fprintf(stderr, "Error: Failed to create index\n");
        return NULL;
    }

    webpage_t* webpage = NULL;
    int docID = 1;

    while((webpage = pagedir_load(pageDirectory, docID))!=NULL) { //Call pagedir_load from pagedir.c
        indexPage(webpage, docID, index);
        docID++;//Increment document ID
        webpage_delete(webpage); // Delete the current page
    }
    return index;
}

/**************** indexPage() ****************/
/* Scans a webpage document and adds its words to the index
 * Step though each word of webpage, normalize words, look up or add word in index
 */
static void indexPage(webpage_t * page, int docID, index_t *index) {
    int pos = 0;
    char* word;

    while ((word = webpage_getNextWord(page, &pos)) != NULL) { //While there is a next word
        if (strlen(word) >= 3) { 
            word = word_normalize(word); // Lowercase
            index_insert(index, word, docID); //Insert the word to the index
        }
        free(word);
        word = NULL;
    }
}
