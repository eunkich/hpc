#ifndef STRASSEN_MM_HPP
#define STRASSEN_MM_HPP

#include <vector>

template <typename T>
std::vector<std::vector<T>> strassen_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B);

#endif // STRASSEN_MM