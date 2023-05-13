#!/bin/bash
mpic++ -std=c++14 -o mpi.o mpi.cpp;
echo "func,val,logerr,time,n,n_process" > multithreads.csv;

for i in {1..8}
do
    mpirun -np $i mpi.o $((10 ** 8));
done

# for i in {1..6}
# do
#     mpirun -np
# done