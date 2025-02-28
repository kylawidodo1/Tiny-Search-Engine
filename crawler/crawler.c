/*
 * crawler.c     Kyla Widodo     April 24, 2023
 *
 * This file contains the source code of crawler
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"


/**************** Function Prototypes ****************/
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** main() ****************/
/* Entry point of program
 * Parses the command-line arguments using the parseArgs function, validates them
 * Calls the crawl function to begin the crawling process
 * Program exits with a non-zero error code if found errors in execution
 */
int main(const int argc, char* argv[]){
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    exit(0);
}

/**************** parseArgs() ****************/
/* Takes in command-line arguments and extracts the seedURL, pageDirectory, and maxDepth
 * Validates seedURL as internal and normalized, page directory exists, and maxDepth is within range
 * Detection of errors prints error message to 'stderr'. Exit non-zero code
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    if (argc != 4) { 
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n"); //Print usage
        exit(1);
    } else { //Assign arguments
        *seedURL = argv[1]; 
        *pageDirectory = argv[2];
        *maxDepth = atoi(argv[3]);

        //Error statements
        if (!normalizeURL(*seedURL) || !isInternalURL(*seedURL)) { 
            fprintf(stderr, "Error: Invalid seed URL\n");
            exit(1);
        }

        if (!pagedir_init(*pageDirectory)) {
            fprintf(stderr, "Error: Failed to initialize page directory\n");
            exit(1);
        }

        if (*maxDepth < 0 || *maxDepth > 10) {
            fprintf(stderr, "Error: Invalid max depth\n");
            exit(1);
        } 
    }
}

/**************** crawl() ****************/
/* Crawls from seedURL to maxDepth and saving pages in pageDirectory
 * Loops over pages to explore, until the list is exhausted
 * Returns void
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    hashtable_t* pagesSeen = hashtable_new(200);

    if(!hashtable_insert(pagesSeen, seedURL, "")) { // If fail to insert hashtable
        fprintf(stderr, "Error: Fail inserting seed URL\n");
        exit(1);
    }
    bag_t* pagesToCrawl = bag_new();
    char* seedCopy= malloc(strlen(seedURL)+1); // Copy URL and make storage
    strcpy(seedCopy, seedURL);
    
    webpage_t* seedPage = webpage_new(seedCopy, 0, NULL);
    if (seedPage == NULL){
        fprintf(stderr, "Error: Unable to create webpage\n");
        exit(1);
    }

    hashtable_insert(pagesSeen, seedURL, ""); //Insert to hashtable
    bag_insert(pagesToCrawl, seedPage);//Insert to bag


    seedPage = bag_extract(pagesToCrawl);
    int documentID = 1; //ID of bags

    while (seedPage != NULL) {
        if (webpage_fetch(seedPage)) {
            fprintf(stderr, "%d Fetched: %s\n", webpage_getDepth(seedPage), webpage_getURL(seedPage)); 
            pagedir_save(seedPage, pageDirectory, documentID); // Save page
            documentID++; // Increment ID


            if (webpage_getDepth(seedPage) < maxDepth) {
                fprintf(stderr, "%d Scannning: %s\n", webpage_getDepth(seedPage), webpage_getURL(seedPage)); 
                pageScan(seedPage, pagesToCrawl, pagesSeen); //Scan page
            }

        } else {
            fprintf(stderr, "Error: Unable to fetch page\n");
            exit(1);
        }
    
        webpage_delete(seedPage); // Delete page
        seedPage=bag_extract(pagesToCrawl); 
    }
    
    hashtable_delete(pagesSeen, NULL); //Delete hashtable
    bag_delete(pagesToCrawl, webpage_delete); //Delete bag
}


/**************** pageScan() ****************/
/* Takes webpage and scans it to extract any URLs, ignore non-internal
 * For each URL not already seen before, adds the URL to pagesSeen and to pagesToCrawl
 * Normalizes the URL to ensure that it is in the correct format
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    int pos = 0;
    char* nextURL;

    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) { // For each extracted URL
        fprintf(stdout, "%d Found: %s\n", webpage_getDepth(page), nextURL);
        if(normalizeURL(nextURL) && isInternalURL(nextURL)) { // If URL normalized and internal
            if (hashtable_insert(pagesSeen, nextURL, "")) { // If added
                fprintf(stdout, "%d Added: %s\n", webpage_getDepth(page), nextURL);
                char* copy= malloc(strlen(nextURL)+1); // Copy into allocated space
                strcpy(copy, nextURL);
                webpage_t* newPage = webpage_new(copy, webpage_getDepth(page) + 1, NULL); // Create new webpage and depth + 1
                bag_insert(pagesToCrawl, newPage); 
            }
            else {
                fprintf(stdout, "%d IgnDupl: %s\n", webpage_getDepth(page), nextURL);
            }
        } else {
            fprintf(stdout, "%d IgnExtrn: %s\n", webpage_getDepth(page), nextURL);
        }
        free(nextURL);
    }
}