export PATH=/gscratch/amath/roche/bin:$PATH
export LD_LIBRARY_PATH=/gscratch/amath/roche/lib:$LD_LIBRARY_PATH

cp oblas_cublas.cpp oblas_cublas.cu;
nvcc -c -arch sm_75 -I/gscratch/amath/roche/include oblas_cublas.cu;
g++ -o xoblas_cublas oblas_cublas.o -L/gscratch/amath/roche/lib -L/sw/cuda/12.0.0/lib64 -lcublas -lcudart -lopenblas -lm
./xoblas_cublas | tee oblas_cublas.csv
 