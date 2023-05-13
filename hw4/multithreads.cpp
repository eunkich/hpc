#include <iostream>
#include <thread>
#include <vector>
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

void compute_partial_sum(double &sum, std::mutex &sum_mutex,
                         double a, double h, int n, int n_threads, int i)
{
    DEBUG ? printf("Start thread %d\n", i) : true;
    double partial_sum = 0.;
    int batch_size = (n / n_threads);
    int start = batch_size * i;
    int end = std::min(n, start + batch_size);

    for (int j = start; j < end; j++)
    {
        double right = a + (j + 1) * h;
        partial_sum += h * f(right);
    }

    // Lock and update the sum variable
    sum_mutex.lock();
    sum += partial_sum;
    sum_mutex.unlock();
    DEBUG ? printf("End thread %d\n", i) : true;
}

double parallel_riemann_sum(double a, double b, int n, int n_threads)
{
    double h = (b - a) / n;
    double sum = 0.;

    std::vector<std::thread> threads(n_threads);

    std::mutex sum_mutex;

    for (int i = 0; i < n_threads; i++)
    {
        threads[i] = std::thread(compute_partial_sum, std::ref(sum),
                                 std::ref(sum_mutex), a, h, n, n_threads, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return sum;
}

int main(int argc, char **argv)
{
    // Number of partitions
    if (argc != 3)
    {
        printf("[Error] Usage: %s [num_partition] [num_threads]\n", argv[0]);
        return 1;
    }
    const int n = atoi(argv[1]);
    const int n_threads = atoi(argv[2]);
    DEBUG ? printf("n = %d, n_threads = %d\n", n, n_threads) : true;
    // const int num_threads = 4;
    double a = 1.;
    double b = 6.;

    // Analytical Solution
    const double ANS = 35. / 8. + log(6);
    DEBUG ? printf("analytical sol. = %.9f\n", ANS) : true;

    // Sequential Calculation
    // auto start = std::chrono::high_resolution_clock::now();
    // double sum_seq = riemann_sum(a, b, n);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    // long double durationL = duration.count() * 1e-9L;
    // DEBUG ? printf("func,val,logerr,time,n,n_threads\n") : true;
    // double logerr = log(abs(ANS - sum_seq));
    // DEBUG ? printf("sum_seq,%.9f,%.9f,%Lf,%d,\n", sum_seq, logerr, durationL, n) : true;

    // Multithread Calculation
    auto start = std::chrono::high_resolution_clock::now();
    double sum_par_threads = parallel_riemann_sum(a, b, n, n_threads);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    long double durationL = duration.count() * 1e-9L;
    double logerr = log(abs(ANS - sum_par_threads));
    printf("sum_par_threads,%.9f,%.9f,%Lf,%d,%d\n", sum_par_threads, logerr, durationL, n, n_threads);
}