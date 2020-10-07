#include "matrix.h"

using namespace task;

Matrix::Matrix() : columns(1), rows(1) {
    initData();
}

Matrix::Matrix(size_t rows, size_t cols) : columns(rows), rows(cols) {
    initData();
}

Matrix::Matrix(const Matrix &copy) {
    columns = copy.columns;
    rows = copy.rows;

    data = new double *[rows];
    for (int i = 0; i < rows; i++) {
        data[i] = new double[columns];
        for (int j = 0; j < columns; j++) {
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

    double **new_data = new double *[a.rows];

    for (int i = 0; i < a.rows; i++) {
        new_data[i] = new double[a.columns];
        for (int j = 0; j < a.columns; j++) {
            new_data[i][j] = a.data[i][j];
        }
    }


    for (int i = 0; i < rows; i++) {
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

    for (int i = 0; i < new_rows; i++) {
        new_data[i] = new double[new_cols];
        for (int j = 0; j < new_cols; j++) {
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

    for (int i = 0; i < rows; i++) {
        delete[] data[i];
    }

    delete[] data;

    columns = new_cols;
    rows = new_rows;
    data = new_data;

}
