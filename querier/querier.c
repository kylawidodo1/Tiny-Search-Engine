/*
 * querier.c     Kyla Widodo     May 8, 2023
 *
 * The querier program  reads the index produced by the Indexer and the page
 * files produced by the Crawler, to interactively answer written queries entered by the user. 
 *
 */

#include <ctype.h>
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
#include <unistd.h>

/**************** Local Structure Types ****************/
typedef struct {
    int docID;
    int score;
} document_t;

struct combined_coutners {            
    counters_t *result;
    counters_t *another;
};

typedef struct doc_array {   //To hold all of the documents that will be printed
    document_t** array;
    int* num;
} doc_array_t;

/**************** Function Prototypes ****************/
int main(const int argc, char* argv[]);

// Parse and Clean
int get_numWords(char* query);
void split_query(char *input, char **words, int num);
static bool parseQuery(char *input, char **query);
static bool checkOperators(char** query, int numWords);
void printQuery(char** query, int numWords);

// Score and Rank
static counters_t* scoreQueries(char** words, int numWords, index_t* index); 
static void rankQueries(counters_t* counter, char* pageDirectory);

static void conjunctionCounters(counters_t *counter1, counters_t *counter2);
static void calculateConjunctionScore(void *arg, const int key, int count);
static void disjunctionCounters(counters_t *counter1, counters_t *counter2);
static void calculateDisjunctionScore(void *arg, const int key, int count);
static inline int min(const int a, const int b);
void sort_helper(void *arg, const int key, int count);

void printDocumentInfo(char* pageDirectory, document_t* document);
static void countNonZeroes(void *arg, const int key, const int count);


/**************** main() ****************/
/* 
 * Entry point of the program. It takes the number of command line arguments and an array 
 of the arguments themselves. It then checks the number of arguments, 
 validates the arguments, loads the index file, reads user queries, processes
  them and ranks the results of the queries. It keeps looping until there are no 
  more lines to read.
 */
int main(const int argc, char* argv[]) {
    // Parse command line
    if (argc != 3) { // Check # of arguments
        fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n"); // Print usage
        exit(1);
    }
    // Assign arguments
    char *pageDirectory = argv[1];
    char *indexFilename = argv[2];

    // Validate pageDirectory and indexFilename -- Error statements
    if (!pagedir_validate(pageDirectory)) { // If not a pathname for a directory produced by crawler
        fprintf(stderr, "Error: pageDirectory doesn't contain valid crawler output\n");
        exit(2);
    }
    FILE* file;
    if ((file = fopen(indexFilename, "r")) == NULL) { // If cannot be read
        fprintf(stderr, "Error: Unable to read a file named indexFilename\n");
        exit(3);
    }

    // Load index from file
    index_t* index = index_load(indexFilename);
    if (index == NULL) {
        fprintf(stderr, "Error: Failed to load index from file\n");
        return 4;
    }

    char* query;
    printf("Query? ");
    while ((query = file_readLine(stdin)) != NULL) { //While there are lines to read
        int numWords = get_numWords(query);
        char** cleanQuery = calloc(numWords, 8);
        if(!parseQuery(query, cleanQuery)){
            free(cleanQuery);
            free(query);
            continue;
        }
        split_query(query, cleanQuery, numWords);

        if (cleanQuery != NULL && numWords != 0) {
            printQuery(cleanQuery, numWords); //Echo pre-processed query

            if (!checkOperators(cleanQuery, numWords)) {
                free(query);
                free(cleanQuery);
                continue;
            } else {
                // Create counters objects of documents
                counters_t* results = scoreQueries(cleanQuery, numWords, index);

                // Ranking results in decreasing order, and print
                rankQueries(results, pageDirectory);
            }
        }
        free(cleanQuery);
        free(query);
    } 
    free(query);
}

/**************** printQuery() ****************/
/* Prints the query to the console.
 *
 * Arguments:
 * - query: Array of strings representing the query words
 * - numWords: Number of words in the query
 */
void printQuery(char** query, int numWords) {
    fprintf(stdout, "Query: ");
    for (int i = 0; i < numWords; i++) {
        if (i == numWords - 1) {
            fprintf(stdout, "%s\n", query[i]); // Print newline after the last word
        } else {
            fprintf(stdout, "%s ", query[i]);
        }
    }
}
/*************** get_numWords ***************/
/*
* counts the number of words in an input string. 
 *It iterates over each character in the input string,
 counting each non-space character that follows a space character as a word. 
 It returns the total number of words.
*/
int get_numWords(char* query) {
    int numWords = 0;
    int check = 0;
    
    for (int i = 0; i < strlen(query); i++) {
        //Count first letter
        if (!isspace(query[i]) && check == 0 ) {
            numWords++;
            check = 1;
        }
        if (isspace(query[i])) {
            check = 0;
        }
    }
    return numWords;
}

/**************** splitQuery() ****************/
/*
* splits a string into individual words. 
* It takes a string and an array of strings 
*/
void split_query(char *input, char **words, int num) {
    char *word = input;
    char *excess = input;

    //loop through the word array and split the words 
    for (int i = 0; i < num; i++)  { 
        while (!isalpha(*word)) {
            word++;
        }
        excess = word;
        while (isalpha(*excess) != 0) {
            excess++;
        }
        *excess = '\0';
        words[i] = word_normalize(word);
        word = excess;
    }
}

 
/**************** checkOperators() ****************/
/* Checks the validity of operators in the query.
 *
 * Arguments:
 * - query: Array of strings representing the query words
 * - numWords: Number of words in the query
 *
 * Returns:
 * - 1 if operators are valid, 0 otherwise.
 */

static bool checkOperators(char** query, int numWords) {
    char* word = query[0];
    char *previous = word;
   
    for (int i = 0; i < numWords; i++) {
        word = query[i];
        
        bool isLiteral = (strcmp(word, "and") == 0) || (strcmp(word, "or") == 0);

        if ((i == 0) && isLiteral) {  
            fprintf(stderr, "Error: '%s' cannot be the first word \n", word);
            printf("Query? ");
            return false;
        } else if (isLiteral && (strcmp(previous, "and") == 0 || strcmp(previous, "or") == 0) && (i != 0)){   
            fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", previous, word);
            printf("Query? ");
            return false;
        } else if ((i == numWords-1) && isLiteral) {  
            fprintf(stderr, "Error: '%s' cannot be the last word\n", word);
            printf("Query? ");
            return false;
        }  
        previous = word;
    }
    return true;
}


/*************** parseQuery ***************/
/*
*  "cleans" a query by checking for invalid characters
*   It takes an input string and an array of strings 
*  (where each element will hold an individual word from the input string). 
*/
static bool parseQuery(char *input, char **query) {
    char *invalid_word = input;
    int non_alpha = 0;
    int spaces = 0;

    while(*invalid_word) { 
        if (isspace(*invalid_word)) { //Empty lines
            spaces++;                       
        } else { 
            if (!isalpha(*invalid_word)) {  //Invalid words
                non_alpha++;
                break;
            }
        }
        invalid_word++;
    }
    // If there are letters, return false
    if (non_alpha > 0 || spaces == strlen(input)) {
        printf("Error: Unaccepted characters in query \n");
        printf("Query? ");
        return false;
    }
    return true;
}


/**************** scoreQueries() ****************/
/* Scores the queries based on the index.
 *
 * Arguments:
 * - words: Array of strings representing the cleaned query words
 * - numWords: Number of words in the query
 * - index: Pointer to the index structure
 *
 * Returns:
 * - Counters object representing the scores of documents for the given query
 */


static counters_t* scoreQueries(char** words, int numWords, index_t* index) {
    counters_t* totalScore = counters_new(); // Track total score
    counters_t* currScore = NULL;

    int check = 0;

    for (int i = 0; i < numWords; i++) {
        if (strcmp(words[i], "and") == 0 ) {
            continue;
        }

        if (check != 0) {                     
            if (!strcmp(words[i], "or")) {
                check = 0;
            }
            continue;
        }

        if (strcmp(words[i], "and") == 0){
            continue;
        }
        if (strcmp(words[i], "or") == 0) {
            if (currScore != NULL) {                 
                conjunctionCounters(currScore, totalScore);
                counters_delete(currScore);           //Reset the currScore
                currScore = NULL;
            }
            continue;
        }

        //if its not a literal, find 
        counters_t* next_word = index_find(index, words[i]);
        if (next_word == NULL) {                            
            check++;
            continue;
        }

        if (currScore == NULL) {  
            currScore = counters_new();               //Conjunct currScore with the found word
            conjunctionCounters(next_word, currScore);
        } else {                                //if there is something in currScore, intersect it with the found word
            disjunctionCounters(currScore, next_word);
        }
    }
    
    if (check == 0) {   //Conjunction if last word not NULL
        conjunctionCounters(currScore, totalScore);
    }
    counters_delete(currScore); //Clean
    
    return totalScore;
}

/**************** conjunctionCounters() ****************/
/* Combines two counters using AND.
 *
 * Arguments:
 * - counter1: Counters object 1
 * - counter2: Counters object 2
 */
static void conjunctionCounters(counters_t *counter1, counters_t *counter2) {
    if (counter1 == NULL || counter2 == NULL) {       
        return;
    } 
    counters_iterate(counter1, counter2, calculateConjunctionScore);
}


/**************** calculateConjunctionScore() ****************/
/* updates the scores of two counters being combined with the AND operation.
*  It takes two counters objects and a key, and adds the counts of the key from both counters.
 *
 */
static void calculateConjunctionScore(void *arg, const int key, int count) {
    counters_t *counter1 = arg;
    int count1 = counters_get(counter1, key);
    if (count1 == 0) {
        counters_set(counter1, key, count);
    } else {
        counters_set(counter1, key, count + count1);
    }
}


/**************** disjunctionCounters() ****************/
/* Combines two counters using OR.
 *
 * Arguments:
 * - counter1: Counters object 1
 * - counter2: Counters object 2
 */
static void disjunctionCounters(counters_t *counter1, counters_t *counter2) {
    if (counter1 == NULL || counter2 == NULL) {      
        return;
    }
    struct combined_coutners args = {counter1, counter2};
    counters_iterate(counter1, &args, calculateDisjunctionScore);
}

/**************** calculateDisjunctionScore() ****************/
/* Updates the scores of the combined counters using OR operation.
 *
 */
static void calculateDisjunctionScore(void *arg, const int key, int count) {
    struct combined_coutners *two = arg;
    counters_set(two->result, key, min(count, counters_get(two->another, key)));    
}

/*************** min ***************/
/*
* returns the smaller of two integers
 It's a helper function used in calculateDisjunctionScore().
*/
static inline int min(const int a, const int b) {
    return (a < b ? a : b);
}

/**************** rankQueries ****************/
/* ranks the queries based on their scores
 It takes a counters object and a page directory as input
  It first counts the number of non-zero scores, then sorts
 the documents based on their scores and finally prints the ranked results.
*/
static void rankQueries(counters_t* counters, char* pageDirectory) {
    int nonZeroCount = 0; // count how many non-zero scores are there
    counters_iterate(counters, &nonZeroCount, countNonZeroes);

    // Create array to hold the documents
    document_t** documents = mem_assert(mem_malloc(nonZeroCount * sizeof(document_t*)), "Unable to allocate memory\n");

    // Fill array with docs
    int num = 0;
    doc_array_t array = {documents, &num};
    counters_iterate(counters, &array, sort_helper);

    if(nonZeroCount == 0) { // if all scores are zero
        fprintf(stdout, "No documents match\n");
        printf("Query? ");
        return;
    } else {
        printf("Matches %d documents (ranked):\n", nonZeroCount);
        printf("—-----------------------------------------------------------------------\n");
     }

    //Print the results
    for (int i = 0; i < nonZeroCount; i++) {      
        char file[strlen(pageDirectory) + 8];
        sprintf(file, "%s/%d", pageDirectory, documents[i]->docID);
        mem_assert(file, "Error: rankQueries failed with invalid file\n");
        
        //Open file and read URL
        FILE* fp = fopen(file, "r");
        if(fp == NULL) {
            fprintf(stderr, "Error: Unable to use file path and read\n");
            return;
        }  

        char *URL = file_readLine(fp);
        fprintf(stdout, "Score: %d, Document %d:, URL: %s\n", documents[i]->score, documents[i]->docID, URL); // Print out the document Info

        //Clean
        fclose(fp);
        free(URL);
    }
    printf("—-----------------------------------------------------------------------\n");
    printf("Query? ");

}

/**************** countNonZeroes ****************/
/* calculates the score for and
*/
static void countNonZeroes(void *arg, const int key, const int count){
    int* countNonZero=arg;
    if(count>0) {
        *countNonZero+=1; // count how many non-zero counts there are
    }
}

/*************** sort_helper ***************/
/*
* Helper function thats sort the individual documents in decrasing order
*/
void sort_helper(void *arg, const int key, int count) {
    if (count > 0) {
        
        doc_array_t *arr = arg;
        document_t *temp = mem_assert(mem_malloc(sizeof(document_t*)), "Memory allocation error\n"); 
        document_t **docs = arr->array;        //Array of documents

        temp->docID = key;
        temp->score = count;
       
        int start = *(arr->num);
        docs[start] = temp;

        for (int i = start - 1; i >= 0; i--) {
            if (docs[i]->score < temp->score) {        //Sorts the documents by score
                docs[i+1] = docs[i];
                docs[i] = temp;                        //Resets the temporary
            }
        }
        *(arr->num) = *(arr->num) + 1;
    }
}
