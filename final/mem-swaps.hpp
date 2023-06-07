#include <vector>

void swapRows(std ::vector<double> &matrix, int nRows, int nCols, int i, int j);
void swapCols(std ::vector<double> &matrix, int nRows, int nCols, int i, int j);

void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nCols; k++)
    {
        std::swap(matrix[i + nRows * k], matrix[j + nRows * k]);
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j)
{
    for (int k = 0; k < nRows; k++)
    {
        std::swap(matrix[k + nRows * i], matrix[k + nRows * j]);
    }
}
