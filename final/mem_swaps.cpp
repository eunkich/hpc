#include <iostream>
#include <vector>
#include <chrono>
#include <random>  // std::rand
#include <utility> // std::pair, std::make_pair
#include "mem_swaps.hpp"

std::pair<int, int> getRandomIndices(int n)
{
    int i = std::rand() % n;
    int j = std::rand() % (n - 1);
    if (j >= i)
    {
        j++;
    }
    return std::make_pair(i, j);
}

void print_matrix(std::vector<double> &matrix, int &M, int &N)
{
    std::cout << "\n";
    for (int r = 0; r < M; r++)
    {
        for (int c = 0; c < N; c++)
        {
            std::cout << matrix[r + M * c] << " ";
        }
        std::cout << "\n";
    }
}

auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
long double elapsed_time = 0.L;

int main()
{
    const int MIN_DIMENSION = 16;
    const int MAX_DIMENSION = 16384;
    const int NTRIALS = 3;

    std::cout << "dim,i,j,swapRows,swapCols\n";

    for (int N = MIN_DIMENSION; N <= MAX_DIMENSION; N *= 2)
    {
        // Square matrix
        int M = N;
        std::vector<double> matrix(M * N, 0.);
        for (int i = 0; i < M * N; i++)
        {
            matrix[i] = static_cast<double>(rand()) / RAND_MAX;
        }

        for (int k = 0; k < NTRIALS; k++)
        {
            std::cout << N;

            std::pair<int, int> rowIndices = getRandomIndices(M);
            int i = rowIndices.first;
            int j = rowIndices.second;

            std::cout << "," << i << "," << j;

            start = std::chrono::high_resolution_clock::now();
            swapRows(matrix, M, N, i, j);
            stop = std::chrono::high_resolution_clock::now();

            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time = duration.count() * 1e-9;
            std::cout << "," << elapsed_time;

            start = std::chrono::high_resolution_clock::now();
            swapCols(matrix, M, N, i, j);
            stop = std::chrono::high_resolution_clock::now();

            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time = duration.count() * 1e-9;
            std::cout << "," << elapsed_time << std::endl;
        }
    }
}