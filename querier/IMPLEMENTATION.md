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

The querier makes use of several data structures including:
- document_t: struct to hold the document ID and score.
- combined_counters: struct to hold two counters objects for conjunction and disjunction calculations.
- doc_array_t: struct to hold all of the documents that will be printed.
- counters_t: A set of counters, each identified by an integer key.
- index_t: An index data structure that maps words to counters.


## Control flow

Main Function
1. Parse command line arguments and validate them.
2. Load index from file.
3. Read query from user.
4. Preprocess query: strip leading and trailing spaces, compact internal spaces, and lowercase all words.
5. Parse and check query.
6. If query is valid, calculate scores and rank queries.
7. Print the ranked results.

Other Functions
- get_numWords(): Counts the number of words in the query.
- split_query(): Splits the query into individual words.
- parseQuery(): Parses the query, checking for invalid characters.
- checkOperators(): Checks if operators are used correctly in the query.
- scoreQueries(): Scores the query based on the index.
- conjunctionCounters(), calculateConjunctionScore(), disjunctionCounters(), calculateDisjunctionScore(): Helper functions for scoreQueries() function to calculate scores.
- rankQueries(): Ranks the queries based on their scores.

### Error Handling
- The querier validates command line arguments, ensuring that the page directory and index file are valid.
- It checks for valid characters in the query and checks for correct usage of operators.
- It handles invalid queries by skipping them and prompting the user for a new query.
- All memory allocations are checked to ensure they are successful.

### main

The main function parses and assigns the arguments, validates pageDirectory and indexFile, then calls queryProcess.

### libcs50

We leverage the modules of libcs50, most notably `hashtable`, and `counters`.
See that directory for module interfaces.

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
int get_numWords(char* query);
void split_query(char *input, char **words, int num);
static bool parseQuery(char *input, char **query);
static bool checkOperators(char** query, int numWords);
void printQuery(char** query, int numWords);
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

```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_validate()` returns false if there is any trouble reading a `.crawler` file or reading the first docuemnt; `pagedir_load()`  returns null when unable to open and read the page file. 

## Testing plan

Here is an implementation-specific testing plan.
1. Test with invalid command line arguments.
2. Test with invalid queries (invalid characters, incorrect operator usage).
3. Test with valid queries, checking the correctness of the results.
4. Test with edge cases (empty queries, single-word queries).
5. Test with large queries to ensure performance.
6. Test with different indexes and page directories.
7. Memory leak checking using Valgrind.

### Regression testing

For routine regression tests, we index the `letters` , `toscrape` and  `wikipedia` site at moderate different depths. Test indextest and indexcmp


### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case 
Third, runs against cs50 index-cmp to check if there are any differences from the desired output
Run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.

