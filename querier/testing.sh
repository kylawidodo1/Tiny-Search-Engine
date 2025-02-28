#!/bin/bash
# testing.sh     Kyla Widodo     May 10, 2023

# Integration testing
# The querier, as a complete program, will be tested by building an index from a pageDirectory

make clean
make

# Test case 1: Incorrect number of arguments
./querier ../data-index/letters-depth-2

# Test case 2: Invalid page directory
./querier invalid_directory ../data-index/indices/letters-2 

# Test case 3: Invalid index filename
./querier ../data-index/letters-depth-2 invalid_filename

# Test case 4: Invalid query syntax
./queri ../data-index/letters-depth-2 ../data-index/indices/letters-2 < test_queries.txt

# Test case 5: Blank Query 
echo "     " | ../data-index/letters-depth-2 ../data-index/indices/letters-2

# Test case 6: Invalid operators
echo "and for or home " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

echo "for or and home " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

echo "for and or home " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

echo "for and home or " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

# Test case 7: Bad Character 
echo "home-for " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

echo "for or fir2t " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

# Test case 8: Nonexistent word in <andsequence>
echo "first and bottle " | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2 

# Test Valid Cases
# Test single word
echo "home" | ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2

# Test <andsequence> Valid
echo "first and search" | valgrind ./querier ../data-index/letters-depth-2 ../data-index/indices/letters-2

# Test valid word sequence 
echo "christian fiction or all products" | valgrind ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index

# Test valid word sequence 
echo "worms or cornerback" | valgrind ./querier ~/cs50-dev/shared/tse/output/wikipedia-2 ~/cs50-dev/shared/tse/indices/wikipedia_2.ndx