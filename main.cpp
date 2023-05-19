#include <iostream>
#include <fstream>
#include <vector>

// Класс для работы с матрицами
template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data; // Двумерный вектор для хранения данных
    int rows; // Число строк
    int cols; // Число столбцов

public:
    // Конструкторы
    Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows, std::vector<T>(cols)) {}

    // Методы для записи и считывания матрицы в/из файла
    void writeToFile(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << rows << " " << cols << "\n";
            for (const auto &row: data) {
                for (const auto &element: row) {
                    file << element << " ";
                }
                file << "\n";
            }
            file.close();
        } else {
            throw std::runtime_error("Unable to open the file.");
        }
    }

    void readFromFile(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> rows >> cols;
            data.resize(rows, std::vector<T>(cols));
            for (auto &row: data) {
                for (auto &element: row) {
                    file >> element;
                }
            }
            file.close();
        } else {
            throw std::runtime_error("Unable to open the file.");
        }
    }

    // Перегрузка операторов
    Matrix<T> operator+(const Matrix<T> &other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Matrix dimensions do not match.");
        }
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator-(const Matrix<T> &other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Matrix dimensions do not match.");
        }
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T> &other) const {
        if (cols != other.rows) {
            throw std::runtime_error("Matrix dimensions are not compatible for multiplication.");
        }
        Matrix<T> result(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix<T> operator*(const T &scalar) const {
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix<T> operator!() const {
        if (rows != cols) {
            throw std::runtime_error("Matrix is not square. Inverse does not exist.");
        }
        Matrix<T> result(rows, cols);
        // код для вычисления обратной матрицы
        // ...
        return result;
    }

    bool operator==(const Matrix<T> &other) const {
        return data == other.data;
    }

    bool operator!=(const Matrix<T> &other) const {
        return data != other.data;
    }

    // Статические методы для создания нулевых и единичных матриц указанного размера
    static Matrix<T> zeros(int rows, int cols) {
        Matrix<T> result(rows, cols);
        return result;
    }

    static Matrix<T> ones(int rows, int cols) {
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = (i == j) ? 1 : 0;
            }
        }
    }
};
