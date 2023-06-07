#include <vector>
#include <fstream>

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j);
void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j);

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nCols; k++)
    {
        // std::swap(matrix[i + nRows * k], matrix[j + nRows * k]);
    }
}

void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nRows; k++)
    {
        // std::swap(matrix[k + nRows * i], matrix[k + nRows * j]);
    }
}
