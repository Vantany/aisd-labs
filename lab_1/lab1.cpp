#include <iostream>
#include <complex>
#include <cstdlib>
#include <ctime>

template <typename T>
class Matrix {
    T** _data;
    size_t _rows, _cols;

public:

    void allocateMemory() {
        _data = new T*[_rows];
        for (size_t i = 0; i < _rows; i++) {
            _data[i] = new T[_cols];
        }
    }

    void deallocateMemory() {
        for (size_t i = 0; i < _rows; i++) {
            delete[] _data[i];
        }
        delete[] _data;
    }

    Matrix(size_t rows, size_t cols, T val) : _rows(rows), _cols(cols) {
        allocateMemory();

        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                _data[i][j] = val;
            }
        }
    }

    Matrix(size_t rows, size_t cols, T lower, T upper) : _rows(rows), _cols(cols) {
        allocateMemory();

        std::srand(static_cast<unsigned int>(std::time(0)));
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                _data[i][j] = lower + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX/(upper - lower)));
            }
        }
    }

    Matrix(const Matrix& other) : _rows(other._rows), _cols(other._cols) {
        allocateMemory();
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; ++j) {
                _data[i][j] = other._data[i][j];
            }
        }
    }

    ~Matrix() { deallocateMemory(); }

    T& operator()(int row, int col) {
        if (row < 0 || row >= _rows || col < 0 || col >= _cols) throw std::logic_error("Неккоректный индекс");
        return _data[row][col];
    }

    bool operator==(const Matrix& other) const {
        if (_rows != other._rows || _cols != other._cols) throw std::logic_error("Вы проверяете на рвенство матрицы разных размеров");
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                if (_data[i][j] != other._data[i][j]) return false;
            }
        }
        return true;
    }
 
    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    Matrix operator+(const Matrix& other) const {
        if (_rows != other._rows || _cols != other._cols) throw std::logic_error("Невозможно выполнить операцию у матриц разных размеров")
        Matrix result(_rows, _cols, T());
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                result._data[i][j] = data[i][j] + other._data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        return this+(-1*other);
    }

    Matrix operator*(const Matrix& other) const {
        if (_cols != other._rows) throw std::invalid_argument("Операция умножения для данных матриц невозможна, т.к. размеры матриц не совпадают");
        Matrix result(_rows, other._cols, T());
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < other._cols; j++) {
                for (size_t k = 0; k < _cols; k++) {
                    result._data[i][j] += data[i][k] * other._data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const {
        Matrix result(_rows, _cols, T());
        for (size_t i = 0; i < _rows; i++) {
            for (size_t j = 0; j < _cols; j++) {
                result[i][j] = _data[i][j] * scalar;
            }
        }
        return result;
    }

    T trace() const {
        if (_rows != _cols) throw std::invalid_argument("След может быть вычислен лишь у квадратной матрицы");
        T tr = T();
        for (size_t i = 0; i < _rows; ++i) {
            tr += _data[i][i];
        }
        return tr;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix) {
        for (int i = 0; i < matrix._rows; i++) {
            for (int j = 0; j < matrix._cols; j++) {
                stream << matrix._data[i][j] << " ";
            }
            stream << "\n";
        }
        return stream;
    }
};


int main() {
    Matrix<float> a(3, 3, 2, 5);
    Matrix<float> b(a);
    std::cout << (a!=b) << std::endl;
}