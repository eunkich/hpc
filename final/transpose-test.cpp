#include <iostream>
#include <vector>
#include "transpose.hpp"

int main()
{
    int M = 4, N = 6;
    std::vector<int> A(M * N, 0);
    for (int i = 0; i < M * N; ++i)
    {
        A[i] = i;
    }

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            std::cout << A[i + M * j] << " ";
        }
        std::cout << "\n";
    }

    // MxN (ColMajor) -> NxM (ColMajor)
    sequentialTranspose(A, M, N);

    std::cout << "\n";
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            std::cout << A[i + N * j] << " ";
        }
        std::cout << "\n";
    }

    // NxM (ColMajor) -> MxN (ColMajor)
    threadedTranspose(A, N, M, 3);

    std::cout << "\n";
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            std::cout << A[i + M * j] << " ";
        }
        std::cout << "\n";
    }
}