# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan


## Data structures 

We use a 'hashtable' that maps from word to (docID, #occurences) pairs. This is the index typestruct. 
The size of the hashtable (slots) is impossible to determine in advance, so we use 200.
We use a counterset to to keep track of number of time a word occurs per docID. 

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function parses and assigns the arguments, validates pageDirectory, then calls `indexBuild` (which calls `indexPage`) then returns zero.

### indexBuild

Do the real work of crawling from `seedURL` to `maxDepth` and saving pages in `pageDirectory`.
Pseudocode:

	set the size of hashtable index
	create a new index object, pass in the size above
    check if index was successfully created
        if not, print error statement
        return non-zero number
    
    create a new webpage 
    set document ID to 1

    loop over the webpage and load page, stop when pagedir_load is null
        pass webpage and docID to the index 
        increment document ID
        delete the page


### indexPage

This function is called by indexBuild. It scans the webpage and adds its words to the index, ensuring the words are normalized
Pseudocode:
    steps through each word of the webpage,
        skips trivial words (less than length 3),
            normalizes the word (converts to lower case),
            adding the word to the index if needed by calling index_insert
        increments the count of occurrences of this word in this docID
        free the word
	

## Other modules

### pagedir

The current re-usable module `pagedir.c` in the separate module, in `../common`, handles the *pagesaver*, and marking it as a Crawler-produced pageDirectory. In order to use indexer, we must add two functions including a function to validate the pagedirectory and another to load the contents of file. 

Pseudocode for `pagedir_validate`:
    return false of the pageDirectory is null
    assert memory of crawler path 
    open crawler file to read 
    check if crawler file is readable
    close crawler file
    
    assert memory of first document path 
    open first docuemnt
    check if frist document (docID 1) is readable 
    close the first document 

    free crawler path
    free first document path 
 

Pseudocode for `pagedir_load`:

	check if pageDirectory does not exit or docID is invalid
    create the docID path and pagePath by allocating memory
    open the page file
    check if null
        Error statement
        return null
    read URL
    read depth string
    read depth int
    read HTML content
    close pagefile
    free paths
    return webpage_new


### libcs50

We leverage the modules of libcs50, most notably `hashtable`, and `counters`.
See that directory for module interfaces.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void index_t* indexBuild(char* pageDirectory, const char* indexFilename);
static void indexPage(webpage_t * page, int docID, index_t *index);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char *pageDirectory);
webpage_t *pagedir_load(const char *pageDirectory, int docID);
```

### word

Detailed descriptions of the function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.

```c
void normalizeWord(char* word);
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.

```c
index_t* index_new(int size);
void index_delete(index_t *index);
bool index_insert(index_t *index, const char *word, int docID);
bool index_increment(index_t* index, const char* word, const int docID);
void index_save(index_t* index, const char* indexFilename);
index_t* index_load(const char* indexFilename);
void* index_find(index_t* index, const char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_validate()` returns false if there is any trouble reading a `.crawler` file or reading the first docuemnt; `pagedir_load()`  returns null when unable to open and read the page file. 

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are only four units (indexer, pagedir, index, word).
The indexer represents the whole system and is covered below.
The pagedir, index, word unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.

### Regression testing

For routine regression tests, we index the `letters` , `toscrape` and  `wikipedia` site at moderate different depths. Test indextest and indexcmp


### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case 
Third, runs against cs50 index-cmp to check if there are any differences from the desired output
Run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.

