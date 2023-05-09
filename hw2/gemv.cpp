#include <iostream>
#include <random>

#include "gemv.hpp"

template <typename T>
void gemv(T a, const std::vector<std::vector<T>> &A,
          const std::vector<T> &x, T b, std::vector<T> &y)
{
    if (x.size() != A.size())
    {
        std::cerr << "Error: matrix A(m x n) and vector x(n x 1) have different sizes" << std::endl;
        return;
    }
    if (y.size() != A[0].size())
    {
        std::cerr << "Error: matrix Ax(m x 1) and vector y(m x 1) have different sizes" << std::endl;
        return;
    }

    for (int i = 0; i < A.size(); i++)
    {
        y[i] *= b;

        T tmp = 0;
        for (int j = 0; j < x.size(); j++)
        {
            tmp += A[i][j] * x[j];
        }

        y[i] += a * tmp;
    }
}

template void gemv(float a, const std::vector<std::vector<float>> &A,
                   const std::vector<float> &x, float b, std::vector<float> &y);

//

// std::chrono::high_resolution_clock::time_point start;
// std::chrono::high_resolution_clock::time_point stop;
// std::chrono::duration<double> duration;

// void test(int N, int n_trials, int option, int blocksize = 0)
// {
//     long double durationL;
//     const long double micro_to_secondL = 1.e-6L;
//     long double flops;
//     long double flop_count;

//     // Theoretical flop counts for each function
//     if (option == 0)
//     {
//         flop_count = static_cast<long double>(2 * N);
//     }
//     else if (option == 1)
//     {
//         flop_count = static_cast<long double>(2 * N);
//     }
//     else if (option == 2)
//     {
//         flop_count = static_cast<long double>((2 * N + 3) * N);
//     }
//     else if (option == 3)
//     {
//         // flop_count = static_cast<long double>((2 * N + 4) * N * N);
//         // (2 * N + 4) * N * N gives overflow when N=1024
//         // Detour: multiply by N after calulating the flops.
//         flop_count = static_cast<long double>((2 * N + 4) * N);
//     }

//     std::vector<long double> performance;

//     // Random number generator
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<float> dist(0.0, 1.0);

//     // Assign random init values
//     float a = dist(gen);
//     float b = dist(gen);

//     std::vector<float> x(N);
//     std::vector<float> y(N);
//     std::vector<float> row(N);
//     std::vector<std::vector<float>> A(N, row), B(N, row), C(N, row);

//     for (int i = 0; i < N; i++)
//     {
//         x[i] = dist(gen);
//         y[i] = dist(gen);
//         for (int j = 0; j < N; j++)
//         {
//             A[i][j] = dist(gen);
//             B[i][j] = dist(gen);
//             C[i][j] = dist(gen);
//         }
//     }

//     // Measure performances
//     for (int i = 0; i < n_trials; i++)
//     {
//         if (option == 2)
//         {
//             start = std::chrono::high_resolution_clock::now();
//             gemv(a, A, x, b, y);
//             stop = std::chrono::high_resolution_clock::now();
//         }

//         duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
//         durationL = static_cast<long double>(duration.count());
//         flops = flop_count / (durationL * micro_to_secondL);
//         if (option == 3)
//         {
//             flops *= N;
//         }

//         if (not isinf(flops))
//         {
//             performance.push_back(flops);
//         }
//         else
//         {
//             std::cerr << "flops is Inf, discarding current sample" << std::endl;
//             i--;
//         }
//     }

//     std::vector<std::string> func_names = {"axpy", "unroll", "gemv", "gemm"};

//     long double sum = 0.0L;
//     for (int i = 0; i < performance.size(); i++)
//     {
//         sum += performance[i];
//     }
//     std::cout << func_names[option] << ", "
//               << N << ", "
//               << n_trials << ", "
//               << sum / performance.size() << ", "
//               << blocksize << std::endl;
// }

// int main()
// {
//     std::cout << "func, N, n_trals, AvgPerf(FLOPs), blocksize\n";
//     int func = 2;
//     if (func == 1) // Different test for axpy_unroll
//     {
//         for (int blocksize = 2; blocksize <= 64; blocksize *= 2)
//         {
//             test(2048, 3, func, blocksize);
//         }
//     }
//     else
//     {
//         for (int N = 2; N <= 1024; N += 2)
//         {
//             test(N, 3, func);
//         }
//     }
//     return 0;
// }