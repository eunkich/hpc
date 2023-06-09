cp gradient_cufft.cpp gradient_cufft.cu; 
nvcc -c -O3 -arch sm_70 gradient_cufft.cu; 
c++ -o xgrad_cufft gradient_cufft.o -L/sw/cuda/12.0.0/lib64 -lcufft -lcudart -lm
./xgrad_cufft | tee grad_cufft.csv