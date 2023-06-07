#include <iostream>
#include <chrono>
#include <cblas.h>        // oblas
#include <cuda_runtime.h> // cuda
#include <cublas_v2.h>    // cublas

auto cstart = std::chrono::high_resolution_clock::now();
auto cstop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cstop - cstart);
long double elapsed_ms_oblas = 0.L;
int main()
{
    // Print header
    std::cout << "dim,trial,ms_cuda,ms_oblas,mflops_cuda,mflops_oblas\n";

    const int MIN_DIMENSION = 16;
    const int MAX_DIMENSION = 8192;
    const int NTRIALS = 3;

    // Allocate host memory for square matrices
    double *h_A = new double[MAX_DIMENSION * MAX_DIMENSION];
    double *h_B = new double[MAX_DIMENSION * MAX_DIMENSION];
    double *h_C = new double[MAX_DIMENSION * MAX_DIMENSION];

    // Create cuBLAS handle
    cublasHandle_t handle;
    cublasCreate(&handle);

    for (int N = MIN_DIMENSION; N <= MAX_DIMENSION; N *= 2)
    {
        double fp_op = 0.;
        fp_op = 2. * N * N * N + 2 * N * N;

        for (int j = 0; j < NTRIALS; j++)
        { // Initialize input matrices
            for (int i = 0; i < N * N; ++i)
            {
                h_A[i] = static_cast<double>(rand()) / RAND_MAX;
                h_B[i] = static_cast<double>(rand()) / RAND_MAX;
                h_C[i] = static_cast<double>(rand()) / RAND_MAX;
            }

            // Allocate device memory for matrices
            double *d_A, *d_B, *d_C;
            cudaMalloc((void **)&d_A, N * N * sizeof(double));
            cudaMalloc((void **)&d_B, N * N * sizeof(double));
            cudaMalloc((void **)&d_C, N * N * sizeof(double));

            // Copy input matrices from host to device
            cudaMemcpy(d_A, h_A, N * N * sizeof(double), cudaMemcpyHostToDevice);
            cudaMemcpy(d_B, h_B, N * N * sizeof(double), cudaMemcpyHostToDevice);

            // Perform matrix multiplication
            const double alpha = static_cast<double>(rand()) / RAND_MAX;
            const double beta = static_cast<double>(rand()) / RAND_MAX;
            cudaEvent_t start, stop;
            cudaEventCreate(&start);
            cudaEventCreate(&stop);
            cudaEventRecord(start, 0);
            cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, N, N, N, &alpha, d_A, N, d_B, N, &beta, d_C, N);
            cudaEventRecord(stop, 0);
            cudaEventSynchronize(stop);

            // Calculate elapsed time
            float elapsed_ms_cublas = 0;
            cudaEventElapsedTime(&elapsed_ms_cublas, start, stop);

            cstart = std::chrono::high_resolution_clock::now();
            cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N, alpha, h_A, N, h_B, N, beta, h_C, N);
            cstop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(cstop - cstart);
            elapsed_ms_oblas = duration.count() * 1.e-6;

            // Free device memory
            cudaFree(d_A);
            cudaFree(d_B);
            cudaFree(d_C);

            // Print performance
            std::cout << N << "," << j << ","
                      << elapsed_ms_cublas << "," << elapsed_ms_oblas << ","
                      << fp_op / 1.e6 / elapsed_ms_cublas << "," << fp_op / 1.e6 / elapsed_ms_oblas << std::endl;
        }
    }

    // Destroy cuBLAS handle
    cublasDestroy(handle);

    // Free host memory
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;

    return 0;
}
