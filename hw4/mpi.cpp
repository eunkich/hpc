#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>
#include <stdio.h>
#include <math.h>

const bool DEBUG = false;

// function to integrate
double f(double x)
{
    return sqrt(1 + (1 / x - x / 4) * (1 / x - x / 4));
}

// Sequential Right Riemann Sum
double riemann_sum(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.;
    for (int i = 0; i < n; i++)
    {
        double right = a + (i + 1) * h;
        sum += h * f(right);
    }
    return sum;
}

int main(int argc, char **argv)
{
    auto start = std::chrono::high_resolution_clock::now();
    int num_process, process_id;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    int n = atoi(argv[1]);

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    // MPI_Get_processor_name(processor_name, &len);
    // DEBUG ? printf("Hello from process %d of %d on %s\n", process_id, num_process, processor_name) : true;

    double a = 1., b = 6.;
    double partition = (b - a) / num_process;

    double local_a = a + process_id * partition;
    double local_b = local_a + partition;
    int local_n = n / num_process;

    double local_sum = riemann_sum(local_a, local_b, local_n);
    DEBUG ? printf("Process %d: Local sum from %f to %f = %f\n", process_id, local_a, local_b, local_sum) : true;

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (process_id == 0)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        long double durationL = duration.count() * 1e-9L;
        DEBUG ? printf("Time: %Lf\n", durationL) : true;

        const double ANS = 35. / 8. + log(6);
        double seq_sum = riemann_sum(a, b, n);
        double logerr = log(abs(ANS - global_sum));

        DEBUG ? printf("Process %d: analytical sol.  = %.9f\n", process_id, ANS) : true;
        DEBUG ? printf("Process %d: sequential appx. = %.9f\n", process_id, seq_sum) : true;
        DEBUG ? printf("Process %d: multiproc appx.  = %.9f\n", process_id, global_sum) : true;
        printf("MPI_Reduce,%.9f,%.9f,%Lf,%d,%d\n", global_sum, logerr, durationL, n, num_process);
    }

    MPI_Finalize();
}