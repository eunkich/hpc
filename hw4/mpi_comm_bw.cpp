#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <random>
#include <mpi.h>
#include <stdio.h>

const bool DEBUG = false;

MPI_Status status;
int num_process, process_id;

std::unordered_map<char, MPI_Datatype> dtype_map = {
    {'i', MPI_INT},
    {'f', MPI_FLOAT},
    {'d', MPI_DOUBLE},
    {'j', MPI_UNSIGNED},
    {'c', MPI_CHAR},
    {'s', MPI_SHORT},
    {'l', MPI_LONG},
    {'m', MPI_UNSIGNED_LONG},
    {'b', MPI_BYTE},
};

template <typename T>
void my_broadcast(T *data, int count, int root, MPI_Comm comm)
{
    MPI_Datatype dtype = MPI_DATATYPE_NULL;
    char name = typeid(T).name()[0];
    dtype = dtype_map[name];

    if (process_id == root)
    {
        // Broadcast from root proc to other procs
        for (int trg = 0; trg < num_process; trg++)
        {
            if (trg == root)
            {
                continue;
            }
            // DEBUG ? printf("Process %d: Sending to %d\n", process_id, trg) : true;
            MPI_Send(data, count, dtype, trg, 0, comm);
        }
    }
    else
    {
        MPI_Recv(data, count, dtype, root, 0, comm, &status);
        // DEBUG ? printf("Process %d: received from %d\n", process_id, root) : true;
    }
}

template <typename T>
void print_buff(T *data, int count)
{
    printf("buffer size: %lu", count * sizeof(data));
    for (int i = 0; i < count; i++)
    {
        printf(", %.2e", data[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    // Configure MPI Comm
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    if (argc != 2)
    {
        (process_id == 0) ? printf("Usage: %s [mesg_size]\n", argv[0]) : true;
        return 1;
    }

    int size = atoi(argv[1]);
    int num_mesg = size / sizeof(double);

    double buff[num_mesg];

    // Prepare messages
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    if (process_id == 0)
    {
        for (auto &e : buff)
        {
            e = dist(rng);
        }
    }
    if (DEBUG)
    {
        // printf("Process %d: Before Bcast| ", process_id);
        // print_buff(buff, num_mesg);
    }

    // point-to-point broadcast
    auto start = std::chrono::high_resolution_clock::now();
    my_broadcast(buff, num_mesg, 0, MPI_COMM_WORLD);
    auto end = std::chrono::high_resolution_clock::now();

    if (DEBUG)
    {
        // printf("Process %d: After  Bcast| ", process_id);
        // print_buff(buff, num_mesg);
    }

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    long double durationL = duration.count() * 1e-9L;
    DEBUG ? printf("Process %d: my_broadcast time = %Lf\n", process_id, durationL) : true;

    // Get maximum duration among all procs
    long double durationL_max;
    MPI_Reduce(&durationL, &durationL_max, 1, MPI_LONG_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (process_id == 0)
    {
        printf("my_broadcast,%lu,%.16Le\n", sizeof(buff), durationL_max);

        // reset buffer
        for (auto &e : buff)
        {
            e = dist(rng);
        }
    }
    if (DEBUG)
    {
        // printf("Process %d: Before Bcast| ", process_id);
        // print_buff(buff, num_mesg);
    }

    // MPI broadcast
    start = std::chrono::high_resolution_clock::now();
    MPI_Bcast(&buff, num_mesg, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    end = std::chrono::high_resolution_clock::now();

    if (DEBUG)
    {
        // printf("Process %d: After  Bcast| ", process_id);
        // print_buff(buff, num_mesg);
    }
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    durationL = duration.count() * 1e-9L;
    DEBUG ? printf("Process %d: MPI_Bcast time = %Lf\n", process_id, durationL) : true;

    MPI_Reduce(&durationL, &durationL_max, 1, MPI_LONG_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (process_id == 0)
    {
        printf("MPI_Bcast,%lu,%.16Le\n", sizeof(buff), durationL_max);
    }

    MPI_Finalize();
}