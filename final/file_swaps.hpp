#include <vector>
#include <fstream>

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j);
void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j);

void swapByIndexInFile(std::fstream &file, int idx_a, int idx_b)
{
    double a, b;

    // Get of the element in the binary file with index
    file.seekg(idx_a * sizeof(double), std::ios::beg);
    file.read(reinterpret_cast<char *>(&a), sizeof(double));

    file.seekg(idx_b * sizeof(double), std::ios::beg);
    file.read(reinterpret_cast<char *>(&b), sizeof(double));

    // Put the elements in the binary file with swapped index
    file.seekp(idx_b * sizeof(double), std::ios::beg);
    file.write(reinterpret_cast<char *>(&a), sizeof(double));

    file.seekp(idx_a * sizeof(double), std::ios::beg);
    file.write(reinterpret_cast<char *>(&b), sizeof(double));
}

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nCols; k++)
    {
        swapByIndexInFile(file, i + nRows * k, j + nRows * k);
    }
}

void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nRows; k++)
    {
        swapByIndexInFile(file, k + nRows * i, k + nRows * j);
    }
}
