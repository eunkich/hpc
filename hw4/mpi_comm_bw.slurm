#!/bin/bash

#SBATCH --job-name=mpi_comm_bw
#SBATCH --mail-type=ALL
#SBATCH --mail-user=eunkich@uw.edu

#SBATCH --account=amath
#SBATCH --partition=gpu-rtx6k
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=40
#SBATCH --mem=10G
#SBATCH --gpus=0
#SBATCH --time=00-00:10:00 # Max runtime in DD-HH:MM:SS format.

#SBATCH --export=all
#SBATCH --output=mpi_comm_bw.csv # where STDOUT goes
#SBATCH --error=mpi_comm_bw.err # where STDERR goes
# Modules to use (optional).
# <e.g., module load singularity>
module load ompi

# Your programs to run.
mpic++ -std=c++14 -o mpi_comm_bw.o mpi_comm_bw.cpp;
echo "func,size,time"

MAX_SIZE=$((128 * 1024 * 1024));

CURR_SIZE=8;

while [ $CURR_SIZE -le $MAX_SIZE ]
do
    mpirun -np 40 ./mpi_comm_bw.o $CURR_SIZE;
    CURR_SIZE=$((2 * $CURR_SIZE))
done

rm *.o
