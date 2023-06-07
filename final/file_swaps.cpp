#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <chrono>

// #include "file_swaps.hpp"

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

// void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j)
// {
//     for (int k = 0; k < nCols; k++)
//     {
//         std::swap(matrix[i + nRows * k], matrix[j + nRows * k]);
//     }
// }

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nCols; k++)
    {
        // std::swap(matrix[i + nRows * k], matrix[j + nRows * k]);
    }
}

int main(int argc, char **argv)
{
    int numRows = 10;
    int numCols = 10;

    // Generate the matrix
    std::vector<double> matrix(numRows * numCols);
    // init matrix elements in column major order
    for (int i = 0; i < numRows * numCols; i++)
    {
        matrix[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    char filename[] = "matrix.bin";

    // write the matrix to a file
    std::fstream file(filename, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<char *>(&matrix[0]), numRows * numCols * sizeof(double));
    file.close();

    // Open the file in read−write mode for swapping
    std::fstream fileToSwap(filename, std::ios::in | std::ios::out | std::ios::binary);

    // Get random indices i and j for row swapping
    std::pair<int, int> rowIndices = getRandomIndices(numRows);
    int i = rowIndices.first;
    int j = rowIndices.second;

    // Measure the time required for row swapping using file I/O
    // auto startTime = std::chrono::high_resolution_clock::now();

    // Swap rows i and j in the file version of the matrix
    // if (fileToSwap.is_open())
    //     fileToSwap >> matrix;
    // swapRowsInFile(fileToSwap, numRows, numCols, i, j);
    // auto endTime = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = endTime - startTime;
    // Close the file after swapping
    // fileToSwap.close();
    // . . .
    // after each problem size delete the test file
    // std::remove(filename.c_str());
    // ...
}
