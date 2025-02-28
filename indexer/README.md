# CS50 Lab 5 — Kyla Widodo 
## CS50 Spring 2021

### indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Usage

The indexer module, defined and implemented in hashtable.c, implement a set of void* to export exactly the following functions: 

```c
int main(const int argc, char* argv[]);
static index_t* indexBuild(char* pageDirectory, index_t* index);
static void indexPage(webpage_t* page, int docID, index_t* index);
```

The indexer's only interface with the user is on the command-line; it must always have two arguments.
`indexer pageDirectory indexFilename`

For example, if letters is a pageDirectory in ../data,
`$ indexer ../data/letters ../data/letters.index`

### Implementation

The key data structure is the index, mapping from word to (docID, #occurrences) pairs. The index is a hashtable keyed by word and storing counters as items. The counters is keyed by docID and stores a count of the number of occurrences of that word in the document with that ID.

### Assumptions

The indexer may assume that:
- `pageDirectory` has files named 1, 2, 3, ..., without gaps.
- The content of files in `pageDirectory` follow the format as defined in the specs.

The index tester may assume that:
- The content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

### Files

* `IMPLEMENTATION.md` - implementation spec
* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `indextest.c` - testing
* `testing.sh` - testing
* `testing.out` - result of `make test &> testing.out`

### Compilation

To build indexer, run `make indexer`
To build indextest, run `make compiletest`

### Testing

The `testing.sh` program 