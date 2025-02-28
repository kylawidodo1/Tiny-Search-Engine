/* 
 * pagedir.h
 *
 * Handles pagesaver (writing a page to the pageDirectory) and 
 * marking page as Crawler-produced pageDirectory
 * 
 * Kyla Widodo, 2023
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include "webpage.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "hashtable.h"


/**************** pagedir_init ****************/
/* Initializes directory with a .crawler file
 * Constructs pathname for the .crawler file in specific directory provided by the caller 
 * Returns true if file succesfully open and close 
 * If error on opening file for writing, return false
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* 
 * Saves the webpage to a file with a unique document ID in the given page directory.
 * The document ID starts at 1 and increments by 1 for each new page.
 * The first line of the file prints URL, the second line prints depth, and the rest of the file is the page content
 * Caller provides: the webpage, existing directory to write webpages, documentID
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* 
 * Verify whether directory is indeed a Crawler-produced directory
 */
bool pagedir_validate(const char *pageDirectory); 

/**************** pagedir_load ****************/
/* 
 * Load a page from a file in the Crawler-produced directory
 * Caller gives: the directory of files already existing, document ID
 */
webpage_t *pagedir_load(const char *pageDirectory, int docID);

#endif // __PAGEDIR_H
