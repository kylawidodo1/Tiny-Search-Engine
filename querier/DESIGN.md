# CS50 TSE Querier
## Design Spec

According to the [Querier Requirements Spec](REQUIREMENTS.md), the TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.


### User interface

The querier is a standalone program that reads the index produced by the Indexer, and the page files produced by the Crawler, to answer written queries entered by the user.

```
querier pageDirectory indexFilename
```
- pageDirectory: The pathname of a directory produced by the Crawler.

- indexFilename: The pathname of a file produced by the Indexer.

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ querier ../data/letters ../data/letters.index
```

### Inputs and outputs

**Input**: 
1. pageDirectory: The pathname of a directory produced by the Crawler.
2. indexFilename: The pathname of a file produced by the Indexer.
3. User query: A valid query entered by the user, containing words and operators. Queries can include any number of words, and the operators 'AND' and 'OR'. The query should not exceed 500 characters.

**Output**: The querier outputs the results of the user's query, listing the matching documents in decreasing order of relevancy. Each document is identified by its document ID and the score that reflects the relevancy of the document to the query. The score is computed based on the frequency of query words appearing in the document. If no documents match the query, the querier will output a message indicating that no matches were found.

Additionally, the querier provides informative error messages if there are issues with user input, such as invalid queries or incorrect usage of operators.

The querier also prints the normalized query to the screen for user verification, to ensure that the input was interpreted correctly.

In the case of invalid pageDirectory or indexFilename, an appropriate error message will be displayed, and the program will terminate with a non-zero exit status.

All output will be directed to the standard output (stdout) in a readable format.

### Functional decomposition into modules

main()
- Parses and validates command-line arguments
- Loads the index from the file
- Reads user queries, processes and normalizes them
- Checks the operators for correctness
- Scores the queries and ranks them
- Frees any allocated memory

printQuery()
- Prints the query that the user entered

get_numWords()
- Returns the number of words in the given query

split_query()
- Splits the query into individual words

checkOperators()
- Checks for operator correctness in the given query
- Returns true if the operators are correct, otherwise false

parseQuery()
- Parses the query
- Checks for invalid characters in the query
- Returns true if the query is valid, otherwise false

scoreQueries()
- Scores the queries based on the index
- Returns a counter of the total score

conjunctionCounters()
- Calculates the conjunction of two counters

calculateConjunctionScore()
- Calculates the conjunction score of a counter

disjunctionCounters()
- Calculates the disjunction of two counters

calculateDisjunctionScore()
- Calculates the disjunction score of a counter

min()
- Returns the minimum of two integers

rankQueries()
- Ranks the queries based on the counters

countNonZeroes()
- Counts the number of non-zero elements in a counter

sort_helper()
- Helps in sorting the queries based on their scores

printDocumentInfo()
- Prints the document information based on the ranking

### Pseudo code for logic/algorithmic flow

function main(args) {
  validate and parse command line args
  load index from file
  prompt user for query
  while there is a query {
    get number of words in the query
    allocate memory for cleanQuery
    if the query is not parseable then free memory and continue
    split the query into cleanQuery
    if cleanQuery is not NULL and numWords is not 0 {
      print the query
      if operators are incorrect then free memory and continue
      score the queries and rank them
    }
    free the query and cleanQuery memory
  }
}

### Major data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 

### Testing plan
To compile: make
You can test the querier by running make test.

