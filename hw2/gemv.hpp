#ifndef GEMV_HPP
#define GEMV_HPP

#include <vector>

template <typename T>
void gemv(T a, const std::vector<std::vector<T>> &A,
          const std::vector<T> &x, T b, std::vector<T> &y);

#endif // GEMV_HPP
