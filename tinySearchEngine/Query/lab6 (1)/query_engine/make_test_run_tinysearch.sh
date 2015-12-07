# 1) go into each directory and make clean, make
# 2) do input testing for crawler - see crawler test script
# 3) crawl at a depth of 3
# 4) Build index at index.dat
# 5) query test prog

# !/bin/bash
# include <stdio.h>

filename="tiny_search_engine_log.txt"

# make and clean each program
cd ../crawler
make clean
make

cd ../indexer
make clean
make

cd ../query_engine
make clean
make

#test crawler
cd ../crawler
echo TEST 1 - wrong number of arguments - OUTPUT: > $filename
./crawler www.cs.dartmouth.edu ./data 1 2 >> $filename
echo TEST 2 - wrong directory - OUTPUT: >> $filename
./crawler www.cs.dartmouth.edu ./dataa 1 >> $filename
echo Running crawler at depth 3... >> $filename
./crawler www.cs.dartmouth.edu/~campbell ./data 3
if [ $? -ne 0 ]; then
	echo Incorrect input to crawler. Exitting program.
	exit EXIT_FAILURE
fi

#build index
cd ../indexer
echo Building index from crawler data... >> ../crawler/$filename
./indexer ../crawler/data indexTEST.dat 
 
# FOR SOME REASON, THE TEST FILE .c IS HAVING A PROBLEM WITH DEFINING
# SHOULD BE SO IT CAN'T RUN RIGHT NOW

# compile call for the test funciton
# gcc query_test.c query.c ../utils/file.c ../utils/list.c ../utils/hash.c -o query_test
# query_test.c

# if [ $? -ne 0 ]; then
# 	echo There was an error in the test file >> $filename
# fi


echo Testing $complete >> $filename













