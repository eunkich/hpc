#include <iostream>
#include <stdexcept>
#include "strassen_mm.hpp"

template <typename T>
std::vector<T> slice(const std::vector<T> &v, const int x, const int y)
{
    auto start = v.begin() + x;
    auto end = v.begin() + y + 1;

    std::vector<T> vector(start, end);
    return vector;
}

template <typename T>
void partition(const std::vector<std::vector<T>> M,
               std::vector<std::vector<T>> &M11,
               std::vector<std::vector<T>> &M12,
               std::vector<std::vector<T>> &M21,
               std::vector<std::vector<T>> &M22)
{
    int N = M.size();
    int cut = M.size() / 2;
    M11 = slice(M, 0, cut - 1);
    for (auto &row : M11)
    {
        row = slice(row, 0, cut - 1);
    }

    M12 = slice(M, 0, cut - 1);
    for (auto &row : M12)
    {
        row = slice(row, cut, N - 1);
    }

    M21 = slice(M, cut, N - 1);
    for (auto &row : M21)
    {
        row = slice(row, 0, cut - 1);
    }

    M22 = slice(M, cut, N - 1);
    for (auto &row : M22)
    {
        row = slice(row, cut, N - 1);
    }
}

template <typename T>
std::vector<std::vector<T>> strassen_mm(const std::vector<std::vector<T>> &A,
                                        const std::vector<std::vector<T>> &B)
{
    // Check input
    bool square = (A.size() == A[0].size()) and (B.size() == B[0].size());
    bool even = (A.size() % 2 == 0) and (B.size() % 2 == 0);
    bool match = A.size() == B.size();
    if (!(square && even && match))
    {
        std::cerr << "Input matrices are not square or not even or dimensions do not match" << std::endl;
        throw std::runtime_error("error");
    }

    int N = A.size();
    // Base case
    if (N == 2)
    {
        T P1, P2, P3, P4, P5, P6, P7;
        P1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
        P2 = (A[1][0] + A[1][1]) * B[0][0];
        P3 = A[0][0] * (B[0][1] - B[1][1]);
        P4 = A[1][1] * (B[1][0] - B[0][0]);
        P5 = (A[0][0] + A[0][1]) * B[1][1];
        P6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
        P7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

        std::vector<T> row(2);
        std::vector<std::vector<T>> C(2, row);

        C[0][0] = P1 + P4 - P5 + P7;
        C[0][1] = P3 + P5;
        C[1][0] = P2 + P4;
        C[1][1] = P1 + P3 - P2 + P6;

        return C;
    }
    else
    {
        std::vector<std::vector<T>> A11, A12, A21, A22;
        std::vector<std::vector<T>> B11, B12, B21, B22;

        partition(A, A11, A12, A21, A22);
        partition(B, B11, B12, B21, B22);

        std::vector<std::vector<T>> _C11, _C12, _C21, _C22;
        std::vector<std::vector<T>> C11_, C12_, C21_, C22_;

        _C11 = strassen_mm(A11, B11);
        C11_ = strassen_mm(A12, B21);
        _C12 = strassen_mm(A11, B12);
        C12_ = strassen_mm(A12, B22);
        _C21 = strassen_mm(A21, B11);
        C21_ = strassen_mm(A22, B21);
        _C22 = strassen_mm(A21, B12);
        C22_ = strassen_mm(A22, B22);

        std::vector<T> row_(N, 0);
        std::vector<std::vector<T>> C(N, row_);
        for (int i = 0; i < N / 2; i++)
        {
            for (int j = 0; j < N / 2; j++)
            {
                C[i][j] = _C11[i][j] + C11_[i][j];
                C[i][j + (N / 2)] = _C12[i][j] + C12_[i][j];
                C[i + (N / 2)][j] = _C21[i][j] + C21_[i][j];
                C[i + (N / 2)][j + (N / 2)] = _C22[i][j] + C22_[i][j];
            }
        }

        return C;
    }
}

template std::vector<std::vector<double>> strassen_mm(const std::vector<std::vector<double>> &A,
                                                      const std::vector<std::vector<double>> &B);
