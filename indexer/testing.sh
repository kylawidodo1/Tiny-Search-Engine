#!/bin/bash
# testing.sh     Kyla Widodo     May 4, 2023

# Integration testing
# The indexer, as a complete program, will be tested by building an index from a pageDirectory,
# and then the resulting index will be validated by running it through the indextest to ensure it can be loaded

make clean

# Function to clean up and create directories
create_directory() {
    if [ -e "$1" ]
    then
        rm -rf "$1"
    fi
    mkdir -p "$1"   
}

# Create directories
create_directory "../data-index"
create_directory "../data-index/letters-depth-2"

cd ../crawler;
make;
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data-index/letters-depth-2 2
cd ../indexer
make all


# Test indexer with various invalid arguments
# 1. No arguments
./indexer

# 2. One argument
./indexer ../data-index/letters-depth-2 

# 3. Three or more arguments 
./indexer ../data-index/letters-depth-2 indexFile1 indexFile2 

# 4. Invalid pageDirectory (non-existent path) 
./indexer ../data-index/l-depth indexFile1

# 5. Invalid pageDirectory (not a crawler directory) 
create_directory "../data-index/notcrawler"
./indexer ../data-index/notcrawler indexFile1

# 6. Invalid indexFile (non-existent path) 
./indexer ../data-index/lettersh-2 ../dat/ldepth

# 7. Invalid indexFile (read-only directory)
./indexer ../data-index/letters-depth-2 ../data-index

# 8. Invalid indexFile (existing, read-only file)
touch rofile
chmod a-w rofile
./indexer ../data-index/letters-depth-2 rofile

rm -rf rofile

# Run indexer on a variety of pageDirectories and use indextest and indexcmp to validate resulting index
create_directory "../data-index/indices"

./indexer ../data-index/letters-depth-2 ../data-index/indices/letters-2
./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data-index/indices/toscrape-1
./indexer ~/cs50-dev/shared/tse/output/wikipedia_2 ../data-index/indices/wikipedia_2

./indextest ../data-index/indices/letters-2 ../data-index/indices/letters-2-copy
./indextest ../data-index/indices/toscrape-1 ../data-index/indices/toscrape-1-copy
./indextest ../data-index/indices/wikipedia_2 ../data-index/indices/wikipedia_2-copy
~/cs50-dev/shared/tse/indexcmp ../data-index/indices/letters-2 ../data-index/indices/letters-2-copy
~/cs50-dev/shared/tse/indexcmp ../data-index/indices/toscrape-1 ../data-index/indices/toscrape-1-copy
~/cs50-dev/shared/tse/indexcmp ../data-index/indices/wikipedia_2 ../data-index/indices/wikipedia_2-copy

# Run valgrind on both indexer and indextest -- Ensure no memory leaks or errors
valgrind ./indexer ../data-index/letters-depth-2 ../data-index/indices/letters-2
valgrind ./indextest ../data-index/indices/letters-2 ../data-index/indices/letters-2-copy