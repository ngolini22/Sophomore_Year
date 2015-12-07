#!/bin/bash
#include <stdio.h>
filename="crawler_testlog.txt"
make clean
make

echo TEST 1 - wrong number of arguments - OUTPUT: > $filename
./crawler www.cs.dartmouth.edu ./data 1 2 >> $filename
echo TEST 2 - wrong directory - OUTPUT: >> $filename
./crawler www.cs.dartmouth.edu ./dataa 1 >> $filename
echo TEST 3 - depth exceeding maximum - OUTPUT: >> $filename
./crawler www.cs.dartmouth.edu ./data 4 >> $filename
echo TEST 4 - incorrect SEED_URL - OUTPUT: >>$filename
./crawler www.cs.artmouth.edu ./data 1 >> $filename

echo Running crawler at depth 0... >> $filename
./crawler www.cs.dartmouth.edu/~campbell ./data 0 
if [ $? -ne 0 ]; then
	echo Incorrect input to crawler. Exitting program.
	exit EXIT_FAILURE
fi

echo Running crawler at depth 1... >> $filename
./crawler www.cs.dartmouth.edu/~campbell ./data 1
if [ $? -ne 0 ]; then
	echo Incorrect input to crawler. Exitting program.
	exit EXIT_FAILURE
fi

# echo Running crawler at depth 2... >> $filename
# ./crawler www.cs.dartmouth.edu/~campbell ./data 2
# if [ $? -ne 0 ]; then
# 	echo Incorrect input to crawler. Exitting program.
# 	exit EXIT_FAILURE
# fi

# echo Running crawler at depth 3... >> $filename
# ./crawler www.cs.dartmouth.edu/~campbell ./data 3
# if [ $? -ne 0 ]; then
# 	echo Incorrect input to crawler. Exitting program.
# 	exit EXIT_FAILURE
# fi
