#include "matrix.h"
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

using namespace task;

Matrix::Matrix() : columns(1), rows(1) {
    initData();
}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), columns(cols) {
    initData();
}

Matrix::Matrix(const Matrix &copy) {
    columns = copy.columns;
    rows = copy.rows;

    data = new double *[rows];
    for (size_t i = 0; i < rows; i++) {
        data[i] = new double[columns];
        for (size_t j = 0; j < columns; j++) {
            data[i][j] = copy.data[i][j];
        }
    }
}


double *Matrix::operator[](size_t row) {
    return data[row];
}

double *Matrix::operator[](size_t row) const {
    return data[row];
}

Matrix &Matrix::operator=(const Matrix &a) {
    if (this == &a)
        return *this;

    auto **new_data = new double *[a.rows];

    for (size_t i = 0; i < a.rows; i++) {
        new_data[i] = new double[a.columns];
        for (size_t j = 0; j < a.columns; j++) {
            new_data[i][j] = a.data[i][j];
        }
    }


    for (size_t i = 0; i < rows; i++) {
        delete[] data[i];
    }

    delete data;

    data = new_data;

    columns = a.columns;
    rows = a.rows;

    return *this;
}

double &Matrix::get(size_t row, size_t col) {
    if (rows < row + 1 || columns < col + 1) {
        throw OutOfBoundsException();
    }

    return data[row][col];
}

const double &Matrix::get(size_t row, size_t col) const {
    if (rows < row + 1 || columns < col + 1) {
        throw OutOfBoundsException();
    }
    return data[row][col];
}

void Matrix::set(size_t row, size_t col, const double &value) {
    if (rows < row + 1 || columns < col + 1) {
        throw OutOfBoundsException();
    }
    data[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {

    double **new_data = new double *[new_cols];

    for (size_t i = 0; i < new_rows; i++) {
        new_data[i] = new double[new_cols];
        for (size_t j = 0; j < new_cols; j++) {
            if (j > columns - 1) {
                new_data[i][j] = 0;
            } else {
                if (i > rows - 1) {
                    new_data[i][j] = 0;
                } else {
                    new_data[i][j] = data[i][j];
                }

            }
        }
    }

    for (size_t i = 0; i < rows; i++) {
        delete[] data[i];
    }

    delete[] data;

    columns = new_cols;
    rows = new_rows;
    data = new_data;

}

std::vector<double> Matrix::getRow(size_t row) {

    std::vector<double> vec(columns);
    for (size_t i = 0; i < columns; i++) {
        vec[i] = data[row][i];
    }
    return vec;
}

std::vector<double> Matrix::getColumn(size_t column) {
    std::vector<double> vec(rows);
    for (size_t i = 0; i < rows; i++) {
        vec[i] = data[i][column];
    }
    return vec;
}

Matrix Matrix::operator*(const Matrix &a) const {
    Matrix m = *this;
    return m *= a;
}

Matrix Matrix::operator*(const double &a) const {
    Matrix m = *this;
    return m *= a;
}

Matrix task::operator*(const double &a, const Matrix &b) {
    Matrix m = b;
    return m *= a;
};

Matrix &Matrix::operator*=(const Matrix &a) {
    if (columns != a.rows)
        throw SizeMismatchException();


    auto **new_data = new double *[rows];

    for (int i = 0; i < rows; i++) {
        new_data[i] = new double[a.columns];
        for (int j = 0; j < a.columns; j++) {
            new_data[i][j] = 0;
            for (int k = 0; k < columns; k++)
                new_data[i][j] += data[i][k] * a[k][j];
        }
    }

    for (size_t i = 0; i < rows; i++) {
        delete[] data[i];
    }

    delete[] data;

    columns = a.columns;
    data = new_data;

    return *this;
}

Matrix &Matrix::operator*=(const double &number) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            data[i][j] = data[i][j] * number;
        }
    }

    return *this;
}

Matrix &Matrix::operator+=(const Matrix &a) {
    if (columns != a.columns || rows != a.rows)
        throw SizeMismatchException();

    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.columns; j++) {
            data[i][j] += a.data[i][j];
        }
    }

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &a) {
    if (columns != a.columns || rows != a.rows)
        throw SizeMismatchException();

    for (size_t i = 0; i < a.rows; i++) {
        for (size_t j = 0; j < a.columns; j++) {
            data[i][j] -= a.data[i][j];
        }
    }

    return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
    Matrix m = *this;
    return m += a;
}

Matrix Matrix::operator-(const Matrix &a) const {
    Matrix m = *this;
    return m -= a;
}

double Matrix::trace() const {
    if (columns != rows)
        throw SizeMismatchException();

    double sum = 0;
    for (int i = 0; i < rows; i++)
        sum += data[i][i];
    return sum;
}

bool Matrix::operator==(const Matrix &a) const {
    if (columns != a.columns || rows != a.rows)
        return false;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            if (std::abs(data[i][j] - a.data[i][j]) > EPS)
                return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix &a) const {
    return !(*this == a);
}

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.columns; j++) {
            output << matrix.data[i][j] << ' ';
        }

        output << std::endl;
    }

    output << std::endl;

    return output;
}


std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
    size_t rows, columns;

    input >> rows >> columns;

    for (size_t i = 0; i < matrix.rows; i++) {
        delete[] matrix.data[i];
    }

    delete[] matrix.data;

    matrix.data = new double *[rows];

    double value;

    for (size_t i = 0; i < rows; i++) {
        matrix.data[i] = new double[columns];
        for (size_t j = 0; j < columns; j++) {
            input >> value;
            matrix.data[i][j] = value;
        }
    }

    matrix.rows = rows;
    matrix.columns = columns;

    return input;
}

Matrix Matrix::operator-() const {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            data[i][j] *= -1;
        }
    }

    return *this;
}

Matrix Matrix::operator+() const {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            data[i][j] *= -1;
        }
    }

    return *this;
}

void Matrix::transpose() {

    auto **new_data = new double *[columns];

    for (int i = 0; i < columns; i++) {
        new_data[i] = new double[rows];
        for (int j = 0; j < rows; j++) {
            new_data[i][j] = data[j][i];
        }
    }

    for (size_t i = 0; i < rows; i++) {
        delete[] data[i];
    }

    delete[] data;

    long tmp = rows;
    rows = columns;
    columns = tmp;
    data = new_data;
}

Matrix Matrix::transposed() const {

    Matrix transposed_matrix(columns, rows);

    for (int i = 0; i < columns; i++) {
        transposed_matrix.data[i] = new double[rows];
        for (int j = 0; j < rows; j++) {
            transposed_matrix.data[i][j] = data[j][i];
        }
    }

    return transposed_matrix;
}

void cofactor(double **mat, double **temp, int p, int q, int n) {
    int i = 0;
    int j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

double calculateDeterminantfMatrix(double **mat, long n) {
    double determinant = 0.;

    if (n == 1)
        return mat[0][0];

    auto **temp = new double *[n];

    for (int i = 0; i < n; i++) {
        temp[i] = new double[n];
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++)
                temp[i][j] = mat[i][j];
        }
    }

    int sign = 1;

    for (long f = 0; f < n; f++) {
        cofactor(mat, temp, 0, f, n);
        determinant += sign * mat[0][f] * calculateDeterminantfMatrix(temp, n - 1);
        sign = -sign;
    }


    for (size_t i = 0; i < n; i++) {
        delete[] temp[i];
    }

    delete[] temp;

    return determinant;
}

double Matrix::det() const {
    if (columns != rows)
        throw SizeMismatchException();
    return calculateDeterminantfMatrix(data, rows);
}
