export PATH=/gscratch/amath/roche/bin:$PATH
export LD_LIBRARY_PATH=/gscratch/amath/roche/lib:$LD_LIBRARY_PATH

cp oblas-cublas.cpp oblas-cublas.cu;
nvcc -c -arch sm_75 -I/gscratch/amath/roche/include oblas-cublas.cu;
g++ -o xoblas-cublas oblas-cublas.o -L/gscratch/amath/roche/lib -L/sw/cuda/12.0.0/lib64 -lcublas -lcudart -lopenblas -lm
./xoblas-cublas | tee oblas-cublas.csv
 