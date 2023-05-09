#include <iostream>
#include <random>
#include <vector>

#include "hw3_p1_header.hpp"

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;
std::chrono::duration<double> duration;

template <typename T>
void init_values(T &a, std::vector<T> &A, std::vector<T> &B,
                 T &b, std::vector<T> &C,
                 int N, std::mt19937 &gen)
{
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    // Assign random init values
    a = dist(gen);
    b = dist(gen);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i + j * N] = dist(gen);
            B[i + j * N] = dist(gen);
            C[i + j * N] = dist(gen);
        }
    }
}

template <typename T>
void test(int N, int n_trials, int option, std::mt19937 &gen,
          T a, std::vector<T> A, std::vector<T> B, T b, std::vector<T> C)
{
    long double durationL;
    const long double micro_to_secondL = 1.e-6L;
    long double flops;
    long double flop_count;

    // Theoretical flop counts for the function
    flop_count = static_cast<long double>((3 * N + 1) * N * N);

    std::vector<long double> performance;

    // Measure performances
    long double flops_sum = 0.0L;

    for (int trial = 0; trial < n_trials; trial++)
    {
        init_values(a, A, B, b, C, N, gen);
        if (option == 0)
        {
            start = std::chrono::high_resolution_clock::now();
            mm_kij(a, A, B, b, C, N, N, N);
            stop = std::chrono::high_resolution_clock::now();
        }
        else if (option == 1)
        {
            start = std::chrono::high_resolution_clock::now();
            mm_jki(a, A, B, b, C, N, N, N);
            stop = std::chrono::high_resolution_clock::now();
        }

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

    std::vector<std::string> func_names = {"mm_kij", "mm_jki"};
    printf("%s, %d, %d, %Lf, ",
           func_names[option].c_str(),
           N,
           n_trials,
           flops_sum / n_trials);
    std::cout << sizeof(a) * 8 << std::endl;
}

int main()
{
    // Random number generator
    int seed;
    std::random_device rd;
    std::mt19937 gen1, gen2;
    seed = rd();
    gen1.seed(seed);
    gen2.seed(seed);

    std::clog << "Run args: seed=" << seed << std::endl;

    // Single Precision
    // Assign variables for operands
    float a, b;
    for (int dim = 2; dim <= 512; dim++)
    {
        std::vector<float> A(dim * dim), B(dim * dim), C(dim * dim);
        test(dim, 3, 0, gen1, a, A, B, b, C);
        test(dim, 3, 1, gen2, a, A, B, b, C);
    }

    // Double Precision
    // Assign variables for operands
    double ad, bd;
    for (int dim = 2; dim <= 512; dim++)
    {
        std::vector<double> Ad(dim * dim), Bd(dim * dim), Cd(dim * dim);
        test(dim, 3, 0, gen1, ad, Ad, Bd, bd, Cd);
        test(dim, 3, 1, gen2, ad, Ad, Bd, bd, Cd);
    }
}