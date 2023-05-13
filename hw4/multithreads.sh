#!/bin/bash
g++ -std=c++14 -o multithreads.o multithreads.cpp
echo "func,val,logerr,time,n,n_threads" > multithreads.csv
for i in {1..16}
do
    ./multithreads.o $((10 ** 8)) $i | tee -a multithreads.csv
done

for i in {1..6}
do
    ./multithreads.o $((10 ** $i)) 4 | tee -a multithreads.csv
done
# ./multithreads.o
# rm *.o