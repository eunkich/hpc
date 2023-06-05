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