#include <iostream>
#include <chrono>
#include <random>
#include <cuda_runtime.h>
#include <cublas_v2.h>

auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
long double elapsed_time = 0.L;

int main()
{
    std::cout << "Direction,size,elapsed_time,bps" << std::endl;
    const int MIN_N = 8 / sizeof(float);
    const int MAX_N = 256 * 1e6 / sizeof(float);

    for (int N = MIN_N; N < MAX_N * 2; N *= 2)
    {
        float *h_A = new float[N];
        for (int i = 0; i < N; i++)
        {
            h_A[i] = static_cast<float>(rand()) / RAND_MAX;
        }

        float *d_A;
        cudaMalloc((void **)&d_A, N * sizeof(float));

        start = std::chrono::high_resolution_clock::now();
        cudaMemcpy(d_A, h_A, N * sizeof(float), cudaMemcpyHostToDevice);
        cudaDeviceSynchronize();
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        elapsed_time = duration.count() * 1e-9;

        // Print performance information
        std::cout << "HostToDevice"
                  << "," << sizeof(float) * N
                  << "," << elapsed_time
                  << "," << (sizeof(float) * N) / elapsed_time
                  << std::endl;

        // Copy array from host to device
        start = std::chrono::high_resolution_clock::now();
        cudaMemcpy(h_A, d_A, N * sizeof(float), cudaMemcpyDeviceToHost);
        cudaDeviceSynchronize();
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        elapsed_time = duration.count() * 1e-9;
        std::cout << "DeviceToHost"
                  << "," << sizeof(float) * N
                  << "," << elapsed_time
                  << "," << (sizeof(float) * N) / elapsed_time
                  << std::endl;

        // Free device memory
        cudaFree(d_A);
        // Free host memory
        delete[] h_A;
    }
}
