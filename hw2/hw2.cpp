#include <iostream>
#include <random>
#include <string>
#include <math.h>

// Level 1 BLAS
void daxpy(double a, const std::vector<double> &x, std::vector<double> &y)
{
    if (x.size() != y.size())
    {
        std::cerr << "Error: vectors have different sizes" << std::endl;
        return;
    }

    for (int i = 0; i < x.size(); i++)
    {
        y[i] += a * x[i];
    }
}

// Level 1 BLAS Unrolled
void daxpy_unroll(double a, const std::vector<double> &x,
                  std::vector<double> &y, int block_size)
{
    if (x.size() != y.size())
    {
        std::cerr << "Error: vectors have different sizes" << std::endl;
        return;
    }

    int unrollDepth = 4;
    const int n = x.size();
    const int nBlocks = n / block_size;

    // Loop over blocks of size blockSize
    for (int iBlock = 0; iBlock < nBlocks; iBlock++)
    {
        const int blockStart = iBlock * block_size;
        const int blockEnd = blockStart + block_size;

        // Compute unrollDepth iterations at a time
        for (int i = blockStart; i < blockEnd; i += unrollDepth)
        {
            const int limit = std::min(i + unrollDepth, blockEnd);
            for (int j = i; j < limit; j++)
            {
                y[j] += a * x[j];
            }
        }
    }

    // Compute remaining elements
    for (int i = nBlocks * block_size; i < n; i++)
    {
        y[i] += a * x[i];
    }
}

// Level 2 BLAS
void dgemv(double a, const std::vector<std::vector<double>> &A,
           const std::vector<double> &x, double b, std::vector<double> &y)
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

        double tmp = 0;
        for (int j = 0; j < x.size(); j++)
        {
            tmp += A[i][j] * x[j];
        }

        y[i] += a * tmp;
    }
}

// Level 3 BLAS
void dgemm(double a, const std::vector<std::vector<double>> &A,
           const std::vector<std::vector<double>> &B, double b,
           std::vector<std::vector<double>> &C)
{
    int m = A.size();
    int p = A[0].size();
    int n = B[0].size();
    if (B.size() != p)
    {
        std::cerr << "Error: matrix dimensions A(m x p) and B(p x n) are not compatible" << std::endl;
        return;
    }
    if (C.size() != m or C[0].size() != n)
    {
        std::cerr << "Error: matrix dimensions AB(m x n) and C(m x n) are not compatible" << std::endl;
        return;
    }

    double tmp;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            tmp = 0;

            for (int k = 0; k < p; k++)
            {
                tmp += A[i][k] * B[k][j];
            }

            tmp *= a;
            C[i][j] *= b;
            C[i][j] += a * tmp;
        }
    }
}

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;
std::chrono::duration<double> duration;

void test(int N, int n_trials, int option, int blocksize = 0)
{
    long double durationL;
    const long double micro_to_secondL = 1.e-6L;
    long double flops;
    long double flop_count;

    // Theoretical flop counts for each function
    if (option == 0)
    {
        flop_count = static_cast<long double>(2 * N);
    }
    else if (option == 1)
    {
        flop_count = static_cast<long double>(2 * N);
    }
    else if (option == 2)
    {
        flop_count = static_cast<long double>((2 * N + 3) * N);
    }
    else if (option == 3)
    {
        // flop_count = static_cast<long double>((2 * N + 4) * N * N);
        // (2 * N + 4) * N * N gives overflow when N=1024
        // Detour: multiply by N after calulating the flops.
        flop_count = static_cast<long double>((2 * N + 4) * N);
    }

    std::vector<long double> performance;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Assign random init values
    double a = dist(gen);
    double b = dist(gen);

    std::vector<double> x(N);
    std::vector<double> y(N);
    std::vector<double> row(N);
    std::vector<std::vector<double>> A(N, row), B(N, row), C(N, row);

    for (int i = 0; i < N; i++)
    {
        x[i] = dist(gen);
        y[i] = dist(gen);
        for (int j = 0; j < N; j++)
        {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
            C[i][j] = dist(gen);
        }
    }

    // Measure performances
    for (int i = 0; i < n_trials; i++)
    {
        if (option == 0)
        {
            start = std::chrono::high_resolution_clock::now();
            daxpy(a, x, y);
            stop = std::chrono::high_resolution_clock::now();
        }
        else if (option == 1)
        {
            start = std::chrono::high_resolution_clock::now();
            daxpy_unroll(a, x, y, 4);
            stop = std::chrono::high_resolution_clock::now();
        }
        else if (option == 2)
        {
            start = std::chrono::high_resolution_clock::now();
            dgemv(a, A, x, b, y);
            stop = std::chrono::high_resolution_clock::now();
        }
        else if (option == 3)
        {
            start = std::chrono::high_resolution_clock::now();
            dgemm(a, A, B, b, C);
            stop = std::chrono::high_resolution_clock::now();
        }

        duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
        durationL = static_cast<long double>(duration.count());
        flops = flop_count / (durationL * micro_to_secondL);
        if (option == 3)
        {
            flops *= N;
        }

        if (not isinf(flops))
        {
            performance.push_back(flops);
        }
        else
        {
            std::cerr << "flops is Inf, discarding current sample" << std::endl;
            i--;
        }
    }

    std::vector<std::string> func_names = {"daxpy", "unroll", "dgemv", "dgemm"};

    long double sum = 0.0L;
    for (int i = 0; i < performance.size(); i++)
    {
        sum += performance[i];
    }
    std::cout << func_names[option] << ", "
              << N << ", "
              << n_trials << ", "
              << sum / performance.size() << ", "
              << blocksize << std::endl;
}

int main()
{
    std::cout << "func, N, n_trals, AvgPerf(FLOPs), blocksize\n";
    for (int func = 0; func < 4; func++)
    {
        if (func == 1) // Different test for axpy_unroll
        {
            for (int blocksize = 2; blocksize <= 64; blocksize *= 2)
            {
                test(2048, 3, func, blocksize);
            }
        }
        else
        {
            for (int N = 2; N <= 1024; N += 1)
            {
                test(N, 3, func);
            }
        }
    }
    return 0;
}