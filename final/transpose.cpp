#include <algorithm> // std::swap
#include <thread>

#include "transpose.hpp"

void sequentialTranspose(std::vector<int> &matrix, int rows, int cols)
{
    // Square matrix
    if (rows == cols)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < i; j++)
            {
                std::swap(matrix[i + rows * j], matrix[j + rows * i]);
            }
        }
    }
    // Nonsquare matrix
    else
    {
        std::vector<int> out(rows * cols, 0);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                out[j + cols * i] = matrix[i + rows * j];
            }
        }
        matrix.swap(out);
    }
}

void partialTranspose(std::vector<int> &matrix,
                      std::vector<std::vector<int>> idx, int rows, int cols)
{
    for (auto &e : idx)
    {
        int org = e[0] + rows * e[1];
        int trg = e[1] + cols * e[0];
        std::swap(matrix[org], matrix[trg]);
    }
}

void partialCopyTranspose(std::vector<int> &matrix, std::vector<int> &copy,
                          std::vector<std::vector<int>> idx, int rows, int cols)
{
    int i, j;
    for (auto &e : idx)
    {
        i = e[0];
        j = e[1];
        copy[j + cols * i] = matrix[i + rows * j];
    }
}

void threadedTranspose(std::vector<int> &matrix, int rows, int cols, int nthreads)
{
    std::vector<std::thread> threads(nthreads);
    std::vector<std::vector<int>> batch;

    // Square matrix
    if (rows == cols)
    {
        int num_swaps = (rows * cols - std::min(rows, cols)) / 2;
        int swap_per_thd = num_swaps / nthreads;
        int tid = 0, num_idx = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = i; j < cols; j++)
            {
                batch.push_back({i, j});
                if (++num_idx == swap_per_thd)
                {
                    if (tid == nthreads - 1)
                    {
                        continue;
                    }
                    threads[tid++] = std::thread(partialTranspose, std::ref(matrix),
                                                 batch, rows, cols);
                    batch.clear();
                    num_idx = 0;
                }
            }
        }
        // Pick up remainder
        threads[tid++] = std::thread(partialTranspose, std::ref(matrix), batch, rows, cols);
        for (auto &thread : threads)
        {
            thread.join();
        }
    }
    // Nonsquare matrix
    else
    {
        std::vector<int> out(rows * cols, 0);
        int num_copy = rows * cols;
        int copy_per_thd = num_copy / nthreads;
        int tid = 0, num_idx = 0;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                batch.push_back({i, j});
                if (++num_idx == copy_per_thd)
                {
                    if (tid == nthreads - 1)
                    {
                        continue;
                    }
                    threads[tid++] = std::thread(partialCopyTranspose, std::ref(matrix),
                                                 std::ref(out), batch, rows, cols);
                    batch.clear();
                    num_idx = 0;
                }
            }
        }
        // Pick up remainder
        threads[tid++] = std::thread(partialCopyTranspose, std::ref(matrix),
                                     std::ref(out), batch, rows, cols);

        for (auto &thread : threads)
        {
            thread.join();
        }
        matrix.swap(out);
    }
}
