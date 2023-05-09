#include <iostream>
#include <random>

#include "strassen_mm.hpp"

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;
std::chrono::duration<double> duration;

template <typename T>
void init_values(std::vector<std::vector<T>> &A,
                 std::vector<std::vector<T>> &B,
                 std::mt19937 &gen)
{
    int N = A.size();
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    // Assign random init values
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }
}

template <typename T>
void test(int N, int n_trials, std::mt19937 &gen,
          std::vector<std::vector<T>> A, std::vector<std::vector<T>> B)
{
    long double durationL;
    const long double micro_to_secondL = 1.e-6L;
    long double flops;
    long double flop_count;

    // Theoretical flop counts for the function
    flop_count = static_cast<long double>(25 * powl(4, (log2(N) - 1)));

    std::vector<long double> performance;

    // Measure performances
    long double flops_sum = 0.0L;

    for (int trial = 0; trial < n_trials; trial++)
    {
        init_values(A, B, gen);
        start = std::chrono::high_resolution_clock::now();
        strassen_mm(A, B);
        stop = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
        durationL = static_cast<long double>(duration.count());
        flops = flop_count / (durationL * micro_to_secondL);

        if (not isinf(flops))
        {
            flops_sum += flops;
        }
        else
        {
            std::cerr << "flops is Inf, discarding current sample" << std::endl;
            trial--;
        }
    }

    printf("%s, %d, %d, %Lf\n",
           "strassen_mm",
           N,
           n_trials,
           flops_sum / n_trials);
    std::cout << std::endl;
}

int main()
{
    // Random number generator
    int seed;
    std::random_device rd;
    std::mt19937 gen;
    seed = rd();
    gen.seed(seed);

    std::clog << "Run args: seed=" << seed << std::endl;

    // Double Precision
    // Assign variables for operands
    for (int dim = 2; dim <= 512; dim *= 2)
    {
        std::vector<double> row(dim);
        std::vector<std::vector<double>> A(dim, row), B(dim, row);
        test(dim, 3, gen, A, B);
    }
}