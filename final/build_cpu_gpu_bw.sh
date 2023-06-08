cp cpu_gpu_bw.cpp cpu_gpu_bw.cu;
nvcc -c -arch sm_75 cpu_gpu_bw.cu;
g++ -o xcpu_gpu_bw cpu_gpu_bw.o -L/sw/cuda/12.0.0/lib64 -lcudart -lm
./xcpu_gpu_bw | tee cpu_gpu_bw.csv
