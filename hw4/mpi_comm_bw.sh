mpic++ -std=c++14 -o mpi_comm_bw.o mpi_comm_bw.cpp
# mpirun -np 8 ./mpi_comm_bw.o $((1024 * 1024));

echo "func,size,time" > mpi_comm_bw.csv
# MAX_SIZE=$((2 * 1024 * 1024));
MAX_SIZE=$((1024 * 1024));

CURR_SIZE=$((8));

while [ $CURR_SIZE -le $MAX_SIZE ]
do
    mpirun -np 6 ./mpi_comm_bw.o $CURR_SIZE;
    CURR_SIZE=$((2 * $CURR_SIZE))
done

rm *.o
