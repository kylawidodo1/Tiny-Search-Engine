# Kyla Widodo
## CS50 Spring 2023, Lab 4

GitHub username: kylawidodo1

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth". It saves the fetched pages to a designated directory.

## Usage:
To run the Crawler program, use the following command format:
`./crawler <seedURL> <pageDirectory> <maxDepth>`
`seedURL` — Starting URL for the crawler. It must be an internal URL.
`pageDirectory` — Directory where fetched pages will be saved. This directory must exist and be writable.
`maxDepth` — Maximum depth to crawl (0-10). A depth of 0 means only the seed URL will be fetched, a depth of 1 means the seed URL and any linked pages will be fetched.

## Definition: 
A normalized URL is the result of passing a URL through normalizeURL(); see the documentation of that function. An Internal URL is a URL that, when normalized, begins with http://cs50tse.cs.dartmouth.edu/tse/.

One example: Http://CS50TSE.CS.Dartmouth.edu//index.html becomes http://cs50tse.cs.dartmouth.edu/index.html.

## Assumption:
The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...).

## Limitation: 
The Crawler shall pause at least one second between page fetches, and shall ignore non-internal and non-normalizable URLs. (The purpose is to avoid overloading our webserver and to avoid causing trouble on any webservers other than the CS50 test server.)

## Testing:
Test script `testing.sh` included to test the Crawler program. 
To run the test = `make test`
To check for memory leaks = `make valgrind` 
To clean up files = `make clean`  
