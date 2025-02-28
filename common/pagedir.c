/*
 * pagedir.c     Kyla Widodo     April 24, 2023
 *
 * This file contains the source code of pagedir
 * Module providing functions ot load webpages from files in the pageDirectory
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"
#include "mem.h"
#include "file.h"

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory) {
    char* crawlerPath = mem_assert(mem_malloc(strlen(pageDirectory) +strlen("/.crawler") +1), "Mem error");
    sprintf(crawlerPath, "%s/.crawler", pageDirectory); // Create the path

    // Open file for writing 
    FILE* crawlerFile = fopen(crawlerPath, "w");
    free(crawlerPath);
    
    if (crawlerFile == NULL) { // Error opening file, return false
        fprintf(stderr, "Error: Unable to open or write in file\n");
        return false;
    }

    fclose(crawlerFile); // Close file
    return true;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) { 
    //Create a file path from directory and ID
    char* documentID = mem_assert(mem_malloc(sizeof(docID)), "Memory error");  //Allocate memory and check if successful
    sprintf(documentID, "%d", docID); // convert docID int to string 
    char* pagePath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen(documentID) + 2, sizeof(char)), "Memory error"); 
    sprintf(pagePath, "%s/%d", pageDirectory, docID);
    
    // Opening file
    FILE* pageFile = fopen(pagePath, "w");
    free(documentID);
    free(pagePath);

    if (pageFile == NULL) { // Error opening directory 
        fprintf(stderr, "Error: Unable to open page directory\n");
        return;
    }
    // Print file contents
    fprintf(pageFile, "%s\n", webpage_getURL(page));
    fprintf(pageFile, "%d\n", webpage_getDepth(page));
    fprintf(pageFile, "%s\n", webpage_getHTML(page));

    fclose(pageFile); //Close file
}

/**************** pagedir_validate() ****************/
/* see pagedir.h for description */
bool pagedir_validate(const char *pageDirectory) {
    if (pageDirectory == NULL) {
        return false;
    }

    char *crawlerPath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen("/.crawler") + 2, sizeof(char)), "Memory error");
    sprintf(crawlerPath, "%s/.crawler", pageDirectory);

    // Check if file exists and is readable
    FILE *crawlerFile = fopen(crawlerPath, "r");
    if (crawlerFile == NULL) {
        fprintf(stderr, "Error: Unable to read a file named pageDirectory/.crawler\n");
        free(crawlerPath);
        return false;
    }

    fclose(crawlerFile);

    // Check if the first document (with ID 1) exists and is readable
    char *firstDocPath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen("/1") + 2, sizeof(char)), "Memory error");
    sprintf(firstDocPath, "%s/1", pageDirectory);

    FILE *firstDocFile = fopen(firstDocPath, "r");
    if (firstDocFile == NULL) {
        fprintf(stderr, "Error: Unable to read a file named pageDirectory/1\n");
        free(firstDocPath);
        return false;
    }

    fclose(firstDocFile);
    free(crawlerPath);
    free(firstDocPath);
    return true;
}

/**************** pagedir_load() ****************/
/* see pagedir.h for description */
webpage_t *pagedir_load(const char *pageDirectory, int docID) {
    if (pageDirectory == NULL || docID < 1) {
        return NULL;
    }

    //Create the file path 
    char* docID_str = mem_assert(mem_malloc(sizeof(docID)), "Memory error");
    sprintf(docID_str, "%d", docID);

    char* pagePath = mem_assert(mem_calloc(strlen(pageDirectory) + strlen(docID_str) + 2, sizeof(char)), "Memory error");
    sprintf(pagePath, "%s/%d", pageDirectory, docID);

    // Open page file
    FILE* pageFile = fopen(pagePath, "r");

    if (pageFile == NULL) {
        free(docID_str);
        free(pagePath);
        return NULL;
    } 

    //Read URL
    char* url = file_readLine(pageFile);
    if (url == NULL) {
        fclose(pageFile);
        return NULL;
    }

    //Read depth string
    char* depth_str = file_readLine(pageFile);
    if (depth_str == NULL) {
        fclose(pageFile);
        free(url);
        return NULL;
    }

    //Read depth int
    int depth = atoi(depth_str);
    free(depth_str);

    // Read HTML content
    char* html = file_readFile(pageFile);
    if (html == NULL) {
        fclose(pageFile);
        free(url);
        return NULL;
    }

    fclose(pageFile);
    free(docID_str);
    free(pagePath);
    return webpage_new(url, depth, html);
}