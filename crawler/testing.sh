
# testing.sh     Kyla Widodo     April 27, 2023

# Test file for crawler.c on the cs50tse webpage


make clean

# Function to clean up and create directories
create_directory() {
    if [ -e "$1" ]
    then
        rm -rf "$1"
    fi
    mkdir -p "$1"
}

make

# Create directories
create_directory "../data"
create_directory "../data/incorrectArgs"

create_directory "../data/letters-depth-0"
create_directory "../data/letters-depth-1"
create_directory "../data/letters-depth-2"
create_directory "../data/letters-depth-10"

create_directory "../data/wikipedia-depth-0"
create_directory "../data/wikipedia-depth-1"
create_directory "../data/wikipedia-depth-2"

create_directory "../data/toscrape-depth-0"
create_directory "../data/toscrape-depth-1"
create_directory "../data/toscrape-depth-2"

create_directory "../data/seed"

# Test with incorrect command-line arguments
echo "Testing with incorrect arguments..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/incorrectArgs
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../dat 2
./crawler http://cs50tse.cs. ../data/incorrectARgs 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data 12

# Test on three CS50 websites with different depths
echo "Testing on letters website with depths 0, 1, 2, 10..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-0 0
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-1 1
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 2
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-10 10
echo ""
# Test different seed page
echo " Testing different seed page..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html ../data/seed 1
echo ""


# Test on toscrape 
echo "Testing on toscrape website with depths 0, 1, 2, 3..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-depth-0 0
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-depth-1 1
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-depth-2 2
echo ""


# Test on wikipedia
echo "Testing on wikipedia website with depths 0, 1, 2..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-depth-0 0
echo ""
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-depth-1 1
echo ""
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-depth-2 2


echo "All tests completed."
