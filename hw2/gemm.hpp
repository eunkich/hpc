#ifndef GEMM_HPP
#define GEMM_HPP

#include <vector>

template <typename T>

void gemm(T a, const std::vector<std::vector<T>> &A,
          const std::vector<std::vector<T>> &B,
          T b, std::vector<std::vector<T>> &C);

#endif // GEMM_HPP
