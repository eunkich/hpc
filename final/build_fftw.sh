g++ -o xgrad_fft -std=c++14 -I/gscratch/amath/roche/include gradient_fftw.cpp -L/gscratch/amath/roche/lib -lfftw3 -lm
./xgrad_fft | tee grad_fft.csv