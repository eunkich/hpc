#include <iostream>
#include <fstream>
#include <random>
#include <vector>

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point stop;
std::chrono::duration<double> duration;

std::mt19937_64 rng(std::random_device{}());
std::uniform_real_distribution<double> dist(0.0, 1.0);

void write_and_read(const int n, const bool DEBUG)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();

    std::vector<double> matrix(n * n);
    for (int i = 0; i < n * n; i++)
    {
        matrix[i] = dist(rng);
    }

    // Print
    if (DEBUG)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << matrix[i + n * j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Write
    start = std::chrono::high_resolution_clock::now();
    std::ofstream outfile("matrix.bin", std::ios::out | std::ios::binary);
    if (outfile.is_open())
    {
        outfile.write(reinterpret_cast<const char *>(matrix.data()), sizeof(double) * n * n);
        outfile.close();
    }
    else
    {
        std::cerr << "Failed to open file: matrix.bin" << std::endl;
        throw 1;
    }
    stop = std::chrono::high_resolution_clock::now();

    // auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double durationL = static_cast<long double>(duration.count());
    durationL *= 1e-9L;
    std::cout << n << "," << sizeof(matrix) / durationL << ",";

    // Read
    start = std::chrono::high_resolution_clock::now();
    std::ifstream input("matrix.bin", std::ios::binary);
    if (!input)
    {
        std::cerr << "Error: could not open the file" << std::endl;
        throw 1;
    }

    std::vector<double> matrix_read(n * n);
    input.read(reinterpret_cast<char *>(matrix_read.data()), sizeof(double) * n * n);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    durationL = static_cast<long double>(duration.count());
    durationL *= 1e-9L;
    std::cout << sizeof(matrix) / durationL << std::setprecision(9) << std::endl;

    // Print
    if (DEBUG)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << matrix_read[i + n * j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " n_dim_min  n_dim_max"
                  << "debug_flag(bool, optional)" << std::endl;
        return 1;
    }

    const int N_MIN = atoi(argv[1]);
    const int N_MAX = atoi(argv[2]);
    const bool DEBUG = argv[3];

    std::cout << "n,write,read" << std::endl;
    for (int n = N_MIN; n <= N_MAX; n *= 2)
    {
        write_and_read(n, DEBUG);
    }

    return 0;
}