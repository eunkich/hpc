g++ -std=c++14 -o io_bandwidth.o io_bandwidth.cpp
./io_bandwidth.o 32 16384 | tee io_bandwidth.csv
rm *.bin
