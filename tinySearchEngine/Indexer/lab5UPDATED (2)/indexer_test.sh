#!/bin/bash
#include <stdio.h>

filename="indexer_testlog.txt"
make clean 
make

echo TEST 1 - wrong number of arguments - OUTPUT: > $filename
./indexer ./data index.dat s 2 >> $filename
echo TEST 2 - check wrong directory - OUTPUT: >> $filename
./indexer ./asdfasdf index.dat >> $filename

file2="index.dat"
file3="new_index.dat"

echo TEST 3 - testing indexer with 4 args, the fourth arg being the filename where new index is written >> $filename
./indexer ./data index.dat new_index.dat >> $filename
echo By having four arguments, index is being written user input filename [index.dat]. >> $filename
echo The following is the first index >> $filename
cat $file2 >> $filename
echo The new index should be saved to user input new_index filename [new_index.dat] >> $filename
echo The following is a diff call on the two indexes >> $filename
diff $file2 $file3
echo If there is nothing inbetween this and the previous sentence, there is no diff >> $filename