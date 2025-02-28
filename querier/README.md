# CS50 Lab 5 — Kyla Widodo 
## CS50 Spring 2021

### querier

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### Usage

The querier module, defined and implemented in querier.c, implement the following functions: 

```c
int main(const int argc, char* argv[]);
```

The querier's only interface with the user is on the command-line; it must always have two arguments.
`./querier pageDirectory indexFilename`

For example, if letters is a pageDirectory in ../data,
`$ ./querier ../data/letters ../data/letters.index`

### Implementation

The key data structure is the index, mapping from word to (docID, #occurrences) pairs. The index is a hashtable keyed by word and storing counters as items. The counters is keyed by docID and stores a count of the number of occurrences of that word in the document with that ID.

The querier may assume that:
- pageDirectory has files named 1, 2, 3, ..., without gaps.
- The content of files in pageDirectory follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.
- The content of the file named by indexFilename follows our index file format (as defined in Lab 5); thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
- The provided index file corresponds to the provided pageDirectory, that is, was built by indexer from the files in that directory.

Querier should:
1. Prints the set of documents that contain all the words in the query; you may treat operators ('and', 'or') as regular words.
2. Supports 'and' and 'or' operators, but without precedence (in mathematical terms, it treats them as left associative, equal precedence operators).
3. Supports 'and' precedence over 'or'.
4. Prints the document set in decreasing order by score

### Files

* `DESIGN.md` - design spec
* `IMPLEMENTATION.md` - implementation spec
* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `testing.sh` - testing
* `testing.out` - result of `make test &> testing.out`

### Compilation

To build querier, run `make querier`

### Testing

The `testing.sh` program 