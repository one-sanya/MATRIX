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

    // Вычисление обратной матрицы
    Matrix<double> inverse() const {
        if (rows != cols) {
            throw std::runtime_error("Matrix is not square. Inverse does not exist.");
        }

        int n = rows;
        Matrix<double> augmented(n, 2 * n);

        // Создание расширенной матрицы, где правая часть является единичной матрицей
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                augmented.data[i][j] = data[i][j];
            }
            augmented.data[i][i + n] = 1.0;
        }

        // Приведение расширенной матрицы к ступенчатому виду с помощью метода Гаусса-Жордана
        for (int i = 0; i < n; i++) {
            // Поиск ведущего элемента в текущем столбце
            int maxRow = i;
            for (int j = i + 1; j < n; j++) {
                if (std::abs(augmented.data[j][i]) > std::abs(augmented.data[maxRow][i])) {
                    maxRow = j;
                }
            }

            // Перестановка строк, чтобы ведущий элемент был на диагонали
            if (maxRow != i) {
                std::swap(augmented.data[i], augmented.data[maxRow]);
            }

            // Деление текущей строки на ведущий элемент
            double pivot = augmented.data[i][i];
            if (pivot == 0.0) {
                throw std::runtime_error("Matrix is singular. Inverse does not exist.");
            }
            for (int j = i; j < 2 * n; j++) {
                augmented.data[i][j] /= pivot;
            }

            // Вычитание текущей строки из остальных строк
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    double factor = augmented.data[j][i];
                    for (int k = i; k < 2 * n; k++) {
                        augmented.data[j][k] -= factor * augmented.data[i][k];
                    }
                }
            }
        }

        // Извлечение обратной матрицы из расширенной матрицы
        Matrix<double> result(n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result.data[i][j] = augmented.data[i][j + n];
            }
        }

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

int main() {
    try {
        // Создание матрицы и запись её в файл
        Matrix<int> matrix(3, 3);
        matrix.writeToFile("matrix.txt");

        // Чтение матрицы из файла
        Matrix<int> newMatrix(0, 0);
        newMatrix.readFromFile("matrix.txt");

        // Сложение матриц
        Matrix<int> sum = matrix + newMatrix;

        // Вычитание матриц
        Matrix<int> diff = matrix - newMatrix;

        // Умножение матриц
        Matrix<int> product = matrix * newMatrix;

        // Умножение матрицы на скаляр
        Matrix<int> scaledMatrix = matrix * 2;

        // Создание нулевой матрицы
        Matrix<int> zerosMatrix = Matrix<int>::zeros(2, 2);

        // Создание единичной матрицы
        Matrix<int> onesMatrix = Matrix<int>::ones(2, 2);

        // Проверка на равенство матриц
        bool isEqual = matrix == newMatrix;

        // Проверка на неравенство матриц
        bool isNotEqual = matrix != newMatrix;
    } catch (const std::exception& e) {
        std::cout << "Exception occurred: " << e.what() << std::endl;
    }

    return 0;
}
