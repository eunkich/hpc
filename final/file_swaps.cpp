#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <chrono>

#include "file_swaps.hpp"

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

int main(int argc, char **argv)
{
    int numRows, numCols;
    const int MIN_DIMENSION = 16;
    const int MAX_DIMENSION = 16384;
    const int NTRIALS = 3;

    std::cout << "dim,i,j,swapRowsInFile,swapColsInFile\n";

    for (int N = MIN_DIMENSION; N <= MAX_DIMENSION; N *= 2)
    {
        numCols = N;
        numRows = N;

        // Generate the matrix
        std::vector<double> matrix(numRows * numCols);
        // init matrix elements in column major order
        for (int i = 0; i < numRows * numCols; i++)
        {
            matrix[i] = static_cast<double>(rand()) / RAND_MAX;
        }

        // print_matrix(matrix, numRows, numCols);

        const char *filename = "matrix.bin";

        // write the matrix to a file
        std::fstream file(filename, std::ios::out | std::ios::binary);
        file.write(reinterpret_cast<char *>(matrix.data()), numRows * numCols * sizeof(double));
        file.close();

        // Open the file in read−write mode for swapping
        std::fstream fileToSwap(filename, std::ios::in | std::ios::out | std::ios::binary);

        for (int k = 0; k < NTRIALS; k++)
        {
            std::cout << N;
            // Get random indices i and j for row swapping
            std::pair<int, int> rowIndices = getRandomIndices(numRows);
            int i = rowIndices.first;
            int j = rowIndices.second;

            // fileToSwap.seekg(0, std::ios::beg); // Reset pointer to the beginning
            // std::vector<double> rmatrix(numRows * numCols);
            // fileToSwap.read(reinterpret_cast<char *>(rmatrix.data()), numRows * numCols * sizeof(double));
            // print_matrix(rmatrix, numRows, numCols);

            std::cout << "," << i << "," << j;

            start = std::chrono::high_resolution_clock::now();
            swapRowsInFile(fileToSwap, numRows, numCols, i, j);
            stop = std::chrono::high_resolution_clock::now();

            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time = duration.count() * 1e-9;
            std::cout << "," << elapsed_time;

            start = std::chrono::high_resolution_clock::now();
            swapColsInFile(fileToSwap, numRows, numCols, i, j);
            stop = std::chrono::high_resolution_clock::now();

            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time = duration.count() * 1e-9;
            std::cout << "," << elapsed_time << std::endl;
        }

        fileToSwap.close();

        // Remove matrix.bin
        std::remove(filename);
    }
}
