g++ -std=c++14 -fPIC -c transpose.cpp;
g++ -std=c++14 -pthread -o xtranspose transpose-test.cpp transpose.o;
ls -lstr xtranspose;
./xtranspose