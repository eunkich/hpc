// k.j.roche - amath 483 / 583 lecture 6
// templates, constructors, destructors, access scope

#include <iostream>
#include <chrono>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

// lecture 6

// simple matrix class
class Matrix
{
public:
    Matrix(int, int);            // constructor
    ~Matrix();                   // destructor
    Matrix(const Matrix &other); // copy constructor

    // accessor methods - class functions that can access private foo
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    double get_ij(int i, int j) const { return matrix_[i][j]; }
    void set_ij(int i, int j, double value) { matrix_[i][j] = value; }
    void print() const;
    // alternate reference notation ... A[i][j]
    // element access operators
    std::vector<double> &operator[](int i) { return matrix_[i]; }
    const std::vector<double> &operator[](int i) const { return matrix_[i]; }

    Matrix operator*(const Matrix &other) const; // matrix multiply
    Matrix operator+(const Matrix &other) const; // matrix addition
    Matrix operator*(double scalar) const;       // scale matrix
    Matrix operator-(const Matrix &other) const; // matrix subtraction is redundant

private:
    std::vector<std::vector<double>> matrix_;
    int rows_;
    int cols_;
};

Matrix::Matrix(int rows, int cols)
    : matrix_(rows, std::vector<double>(cols)), rows_(rows), cols_(cols)
{ // matrix constructor
}

Matrix::~Matrix()
{ // deallocate the memory used by the vector of vectors
    matrix_.clear();
    std::cout << "Matrix destructed" << std::endl;
}

Matrix::Matrix(const Matrix &other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_)
{ // copy constructor
}

void Matrix::print() const
{
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            std::cout << matrix_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::operator*(const Matrix &other) const
{ // operator overload multiplication operator *, A * B
    if (cols_ != other.rows_)
    {
        throw std::invalid_argument("Matrices are not compatible for multiplication");
    }
    Matrix result(rows_, other.cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < other.cols_; j++)
        {
            double sum = 0;
            for (int k = 0; k < cols_; k++)
            {
                sum += matrix_[i][k] * other.matrix_[k][j];
            }
            result.set_ij(i, j, sum);
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix &other) const
{ // operator overload addition operator +, A + B
    if (rows_ != other.rows_ || cols_ != other.cols_)
    {
        throw std::invalid_argument("Matrices are not compatible for addition");
    }
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.set_ij(i, j, matrix_[i][j] + other.matrix_[i][j]);
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const
{ // scalar multiplication on matrix elements *, a * A
    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.matrix_[i][j] = scalar * matrix_[i][j];
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix &matrix)
{ // scalar multiplication on matrix elements *, A * a
    return matrix * scalar;
}

Matrix Matrix::operator-(const Matrix &other) const
{ // operator overload addition operator -, A - B
    if (rows_ != other.rows_ || cols_ != other.cols_)
    {
        throw std::invalid_argument("Matrix dimensions must match");
    }

    Matrix result(rows_, cols_);
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < cols_; j++)
        {
            result.set_ij(i, j, matrix_[i][j] - other.matrix_[i][j]);
        }
    }

    return result;
}

// ***************************

class rectangle
{
public:
    int x, y;
    int area();                   // method declaration
    rectangle();                  // constructor
    rectangle(int, int);          // overload constructor
    rectangle(const rectangle &); // copy constructor
    ~rectangle();                 // destructor
};
rectangle::rectangle() { std::cout << "rectangle constructed" << std::endl; }

rectangle::rectangle(int a, int b) : x(a), y(b)
{
    std::cout << "rectangle overload constructor" << std::endl;
}

rectangle::rectangle(const rectangle &other) : x(other.x), y(other.y)
{
    std::cout << "rectangle copy constructor" << std::endl;
}

rectangle::~rectangle() { std::cout << "rectangle destructed" << std::endl; }

int rectangle::area() { return x * y; }

// inheritance ...
class square : public rectangle
{
};
// rectangle is the base class of square
// square is derived from rectangle
// square does not define any new member
// functions or variables,
// but it can use all of the member functions
// and variables of rectangle

// multiple inheritance
class people
{
};
class employee
{
};
class professor : public people, public employee
{
};

/*
std::vector<int> vectorSum(
    const std::vector<int> &a,
    const std::vector<int> &b)
{ // simple function for adding integer vector types
    if (a.size() != b.size())
    {
        std::cerr << "Error: vectors have different sizes" << std::endl;
        return std::vector<int>(); // Return an empty vector
    }

    std::vector<int> c(a.size()); // Result vector

    // Perform vector addition
    for (int i = 0; i < a.size(); i++)
    {
        c[i] = a[i] + b[i];
    }

    return c;
}
std::vector<double> vectorSum(
    const std::vector<double> &a,
    const std::vector<double> &b)
{
    if (a.size() != b.size())
    {
        std::cerr << "Error: vectors have different sizes" << std::endl;
        return std::vector<double>(); // Return an empty vector
    }

    std::vector<double> c(a.size()); // Result vector

    // Perform vector addition
    for (int i = 0; i < a.size(); i++)
    {
        c[i] = a[i] + b[i];
    }

    return c;
}
*/
// this template replaces the need for the two functions on the same name commented above
// templates typename
template <typename T>
std::vector<T> vectorSum(
    const std::vector<T> &a,
    const std::vector<T> &b)
{
    if (a.size() != b.size())
    {
        std::cerr << "Error: vectors have different sizes" << std::endl;
        return std::vector<T>(); // Return an empty vector
    }

    std::vector<T> c(a.size()); // Result vector

    // Perform vector addition
    for (int i = 0; i < a.size(); i++)
    {
        c[i] = a[i] + b[i];
    }

    return c;
}

void kswp(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

// templates class
template <class T>
void kswp(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// variable template
template <class T>
constexpr T pi = T(3.1415926535897932384626433L);

// non-type parameters
template <class T = int, const int n = 3>
class myStorage
{
public:
    T store[n];
};
myStorage<> default_box;
myStorage<double, 10> dbox;

int main()
{
    rectangle r1;
    r1.x = 3;
    r1.y = 4;
    std::cout << "area: " << r1.area() << std::endl;

    square s1;
    s1.x = 4;
    s1.y = 5;
    std::cout << "area s1: " << s1.area() << std::endl;

    // allows accessing only the public members of rectangle
    rectangle &r6 = s1;  // reference upcast
    rectangle *r7 = &s1; // pointer upcast

    // downcast by explicit cast
    square &sq1 = static_cast<square &>(r6);
    square *sq2 = static_cast<square *>(r7);

    // return 0; // fast return

    rectangle r3(4, 5);
    std::cout << "area r3: " << r3.area() << std::endl;

    rectangle r4 = rectangle(r3); // shallow copy
    std::cout << "area r4: " << r4.area() << std::endl;
    /*
    r4.x = 7; r4.y = 9;
    std::cout << "area r4: " << r4.area() << std::endl;
    std::cout << "area r3: " << r3.area() << std::endl;
    */

    rectangle *r5 = new rectangle(r1);
    std::cout << "area r5: " << r5->area() << std::endl;

    (*r5).x = 6;
    (*r5).y = 7;
    std::cout << "area r5: " << r5->area() << std::endl;
    std::cout << "area r1: " << r1.area() << std::endl;

    delete r5; // clean up the heap
    // return 0;  // fast return

    rectangle r2;
    r2.x = 3;
    r2.y = 5;
    std::cout << "area: " << r2.area() << std::endl;

    // use of pointer
    rectangle *pr = &r2;
    std::cout << "area (pointer use): " << pr->area() << std::endl;

    // modify with pointer ...
    pr->x = 6;
    pr->y = 8;
    std::cout << "area (pointer use): " << pr->area() << std::endl;

    // equivalent notation ...
    (*pr).x = 5;
    (*pr).y = 10;
    std::cout << "area (pointer use): " << (*pr).area() << std::endl;

    // start lecture 6 material
    //  let's look at templates ...
    // integer
    std::vector<int> a = {1, 2, 3};
    std::vector<int> d = {4, 5, 6, 7};
    std::vector<int> c = vectorSum(a, d); // throw an error
    std::vector<int> b = {4, 5, 6};
    c = vectorSum(a, b);
    // Check if the result vector is empty
    if (c.empty())
        return 1; // Return an error code
    // Output the result vector
    for (int i = 0; i < c.size(); i++)
        std::cout << c[i] << " ";
    std::cout << std::endl;

    // double
    std::vector<double> x = {1.5, 2.0, 3.5};
    std::vector<double> y = {4.0, 5.5, 6.0};
    std::vector<double> z = vectorSum(x, y);
    if (z.empty())
        return 1; // Return an error code
    for (int i = 0; i < z.size(); i++)
        std::cout << z[i] << " ";
    std::cout << std::endl;

    // use of template class for swapping class rectangle ...
    std::cout << "Before swapping:" << std::endl;
    std::cout << "r1.x = " << r1.x << ", r1.y = " << r1.y << std::endl;
    std::cout << "r2.x = " << r2.x << ", r2.y = " << r2.y << std::endl;
    kswp<rectangle>(r1, r2); // swap r1 and r2
    // kswp(r1, r2);
    std::cout << "After swapping:" << std::endl;
    std::cout << "r1.x = " << r1.x << ", r1.y = " << r1.y << std::endl;
    std::cout << "r2.x = " << r2.x << ", r2.y = " << r2.y << std::endl;

    bool S1 = true;
    bool S2 = false;
    std::cout << "Before swapping boolean:" << std::endl;
    std::cout << "S1:" << (S1 == 1 ? "true" : "false") << std::endl;
    std::cout << "S2:" << (S2 == 1 ? "true" : "false") << std::endl;

    kswp<bool>(S1, S2); // swap S1 and S2

    std::cout << "After swapping boolean:" << std::endl;
    std::cout << "S1:" << (S1 == 1 ? "true" : "false") << std::endl;
    std::cout << "S2:" << (S2 == 1 ? "true" : "false") << std::endl;

    // variable template
    std::cout << "Pi: " << pi<int> << std::endl;
    std::cout << "Pi: " << pi<float> << std::endl;
    std::cout << "Pi: " << std::setprecision(15) << pi<double> << std::endl;

    // example of simple matrix class with operator overloading
    // Initialize matrices
    Matrix A(2, 2); // constructor invoked ...
    A.set_ij(0, 0, 1.0);
    A.set_ij(0, 1, 2.0);
    A.set_ij(1, 0, 3.0);
    A.set_ij(1, 1, 4.0);

    Matrix F(A); // copy A using the copy constructor

    // check the alternate access notation
    double v = A[0][1];
    std::cout << "w = A[0][1] = " << v << std::endl;

    Matrix B(2, 2);
    B.set_ij(0, 0, 5.0);
    B.set_ij(0, 1, 6.0);
    B.set_ij(1, 0, 7.0);
    B.set_ij(1, 1, 8.0);

    // Matrix multiplication
    Matrix C = A * B;
    std::cout << "Matrix C = A * B:" << std::endl;
    C.print();

    // Matrix addition
    Matrix D = A + B;
    std::cout << "Matrix D = A + B:" << std::endl;
    D.print();

    // Scalar multiplication
    Matrix E = A * 2.0;
    std::cout << "Matrix E = 2. * A:" << std::endl;
    E.print();

    return 0;
}