#include <iostream>
#include <random>

#include "gemm.hpp"

template <typename T>
void gemm(T a, const std::vector<std::vector<T>> &A,
          const std::vector<std::vector<T>> &B,
          T b, std::vector<std::vector<T>> &C)
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

    T tmp;
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

template void gemm(float a, const std::vector<std::vector<float>> &A,
                   const std::vector<std::vector<float>> &B,
                   float b, std::vector<std::vector<float>> &C);
