#ifndef TRANSPOSE
#define TRANSPOSE

#include <vector>

void sequentialTranspose(std::vector<int> &matrix, int rows, int cols);
void threadedTranspose(std::vector<int> &matrix, int rows, int cols, int nthreads);

#endif // TRANSPOSE
