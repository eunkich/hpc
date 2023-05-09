date;
rm strassen_*.o;
g++ -std=c++14 -c -fPIC strassen_mm.cpp;
g++ -std=c++14 -o strassen_test strassen_test.cpp strassen_mm.o
echo "Built program binary"
ls -lstr strassen_test;