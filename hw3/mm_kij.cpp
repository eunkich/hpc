#include <iostream>
#include <random>

#include "mm_kij.hpp"

template <typename T>
void mm_kij(T a, const std::vector<T> &A, const std::vector<T> &B, T b,
            std::vector<T> &C, int m, int p, int n)
{
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < p; j++)
            {
                if (j == 0)
                {
                    C[i + k * m] *= b;
                }
                C[i + k * m] += a * A[i + j * m] * B[j + k * p];
            }
        }
    }
}

template void mm_kij(double a, const std::vector<double> &A,
                     const std::vector<double> &B, double b,
                     std::vector<double> &C,
                     int m, int p, int n);
template void mm_kij(float a, const std::vector<float> &A,
                     const std::vector<float> &B, float b,
                     std::vector<float> &C,
                     int m, int p, int n);
