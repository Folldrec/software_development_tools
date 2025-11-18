#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cmath>

template<typename T>
class SparseMatrix {
protected:
    size_t rows, cols;
    T defaultValue;
    
public:
    SparseMatrix(size_t r = 0, size_t c = 0, const T& defVal = T())
        : rows(r), cols(c), defaultValue(defVal) {}
    
    virtual ~SparseMatrix() = default;
    
    virtual T get(size_t row, size_t col) const = 0;
    virtual void set(size_t row, size_t col, const T& value) = 0;
    virtual size_t nonZeroCount() const = 0;
    virtual std::string toString() const = 0;
    virtual void clear() = 0;
    
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    
    virtual SparseMatrix<T>* add(const SparseMatrix<T>& other) const = 0;
    virtual SparseMatrix<T>* multiply(const SparseMatrix<T>& other) const = 0;
    virtual std::vector<T> multiplyVector(const std::vector<T>& vec) const = 0;
    virtual SparseMatrix<T>* transpose() const = 0;
    
    virtual void saveToFile(const std::string& filename) const = 0;
    virtual void loadFromFile(const std::string& filename) = 0;
};

template<typename T>
class MapSparseMatrix : public SparseMatrix<T> {
private:
    std::map<std::pair<size_t, size_t>, T> data;
    
    using SparseMatrix<T>::rows;
    using SparseMatrix<T>::cols;
    using SparseMatrix<T>::defaultValue;
    
public:
    MapSparseMatrix(size_t r = 0, size_t c = 0, const T& defVal = T())
        : SparseMatrix<T>(r, c, defVal) {}
    
    T get(size_t row, size_t col) const override {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Matrix index out of range");
        }
        auto it = data.find({row, col});
        return (it != data.end()) ? it->second : defaultValue;
    }
    
    void set(size_t row, size_t col, const T& value) override {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Matrix index out of range");
        }
        
        if (value == defaultValue) {
            data.erase({row, col});
        } else {
            data[{row, col}] = value;
        }
    }
    
    size_t nonZeroCount() const override {
        return data.size();
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << "MapSparseMatrix[" << rows << "x" << cols << ", stored=" << data.size() << "]:\n";
        
        size_t displayRows = std::min(rows, size_t(5));
        size_t displayCols = std::min(cols, size_t(10));
        
        for (size_t i = 0; i < displayRows; ++i) {
            oss << "[";
            for (size_t j = 0; j < displayCols; ++j) {
                if (j > 0) oss << " ";
                oss << get(i, j);
            }
            if (cols > displayCols) oss << " ...";
            oss << "]\n";
        }
        if (rows > displayRows) oss << "...\n";
        
        return oss.str();
    }
    
    void clear() override {
        data.clear();
    }
    
    SparseMatrix<T>* add(const SparseMatrix<T>& other) const override {
        if (rows != other.getRows() || cols != other.getCols()) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }
        
        MapSparseMatrix<T>* result = new MapSparseMatrix<T>(rows, cols, defaultValue);
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                T sum = get(i, j) + other.get(i, j);
                if (sum != defaultValue) {
                    result->set(i, j, sum);
                }
            }
        }
        
        return result;
    }
    
    SparseMatrix<T>* multiply(const SparseMatrix<T>& other) const override {
        if (cols != other.getRows()) {
            throw std::invalid_argument("Invalid dimensions for matrix multiplication");
        }
        
        MapSparseMatrix<T>* result = new MapSparseMatrix<T>(rows, other.getCols(), defaultValue);
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.getCols(); ++j) {
                T sum = defaultValue;
                for (size_t k = 0; k < cols; ++k) {
                    sum = sum + get(i, k) * other.get(k, j);
                }
                if (sum != defaultValue) {
                    result->set(i, j, sum);
                }
            }
        }
        
        return result;
    }
    
    std::vector<T> multiplyVector(const std::vector<T>& vec) const override {
        if (cols != vec.size()) {
            throw std::invalid_argument("Vector size must match matrix columns");
        }
        
        std::vector<T> result(rows, defaultValue);
        for (size_t i = 0; i < rows; ++i) {
            T sum = defaultValue;
            for (size_t j = 0; j < cols; ++j) {
                sum = sum + get(i, j) * vec[j];
            }
            result[i] = sum;
        }
        
        return result;
    }
    
    SparseMatrix<T>* transpose() const override {
        MapSparseMatrix<T>* result = new MapSparseMatrix<T>(cols, rows, defaultValue);
        
        for (const auto& entry : data) {
            result->set(entry.first.second, entry.first.first, entry.second);
        }
        
        return result;
    }
    
    void saveToFile(const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing");
        
        out << "MapSparseMatrix\n";
        out << rows << " " << cols << "\n";
        out << data.size() << "\n";
        for (const auto& entry : data) {
            out << entry.first.first << " " << entry.first.second << " " << entry.second << "\n";
        }
    }
    
    void loadFromFile(const std::string& filename) override {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Cannot open file for reading");
        
        std::string type;
        in >> type;
        if (type != "MapSparseMatrix") throw std::runtime_error("Invalid file format");
        
        size_t r, c, count;
        in >> r >> c >> count;
        
        rows = r;
        cols = c;
        data.clear();
        
        for (size_t i = 0; i < count; ++i) {
            size_t row, col;
            T val;
            in >> row >> col >> val;
            data[{row, col}] = val;
        }
    }
    
    void generateRandom(size_t r, size_t c, double density, std::function<T()> generator) {
        rows = r;
        cols = c;
        data.clear();
        
        size_t totalElements = r * c;
        size_t count = static_cast<size_t>(totalElements * density);
        
        for (size_t i = 0; i < count; ++i) {
            size_t row = rand() % r;
            size_t col = rand() % c;
            data[{row, col}] = generator();
        }
    }
};

template<typename T>
class CSRSparseMatrix : public SparseMatrix<T> {
private:
    std::vector<T> values;
    std::vector<size_t> colIndices;
    std::vector<size_t> rowPointers;
    
    using SparseMatrix<T>::rows;
    using SparseMatrix<T>::cols;
    using SparseMatrix<T>::defaultValue;
    
public:
    CSRSparseMatrix(size_t r = 0, size_t c = 0, const T& defVal = T())
        : SparseMatrix<T>(r, c, defVal) {
        rowPointers.resize(r + 1, 0);
    }
    
    T get(size_t row, size_t col) const override {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Matrix index out of range");
        }
        
        size_t start = rowPointers[row];
        size_t end = rowPointers[row + 1];
        
        for (size_t i = start; i < end; ++i) {
            if (colIndices[i] == col) {
                return values[i];
            }
        }
        
        return defaultValue;
    }
    
    void set(size_t row, size_t col, const T& value) override {
        throw std::runtime_error("CSR set not implemented - use for read-only operations");
    }
    
    size_t nonZeroCount() const override {
        return values.size();
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << "CSRSparseMatrix[" << rows << "x" << cols << ", stored=" << values.size() << "]";
        return oss.str();
    }
    
    void clear() override {
        values.clear();
        colIndices.clear();
        rowPointers.assign(rows + 1, 0);
    }
    
    SparseMatrix<T>* add(const SparseMatrix<T>& other) const override {
        throw std::runtime_error("CSR operations not fully implemented");
    }
    
    SparseMatrix<T>* multiply(const SparseMatrix<T>& other) const override {
        throw std::runtime_error("CSR operations not fully implemented");
    }
    
    std::vector<T> multiplyVector(const std::vector<T>& vec) const override {
        if (cols != vec.size()) {
            throw std::invalid_argument("Vector size must match matrix columns");
        }
        
        std::vector<T> result(rows, defaultValue);
        for (size_t i = 0; i < rows; ++i) {
            T sum = defaultValue;
            size_t start = rowPointers[i];
            size_t end = rowPointers[i + 1];
            
            for (size_t j = start; j < end; ++j) {
                sum = sum + values[j] * vec[colIndices[j]];
            }
            result[i] = sum;
        }
        
        return result;
    }
    
    SparseMatrix<T>* transpose() const override {
        throw std::runtime_error("CSR transpose not implemented");
    }
    
    void saveToFile(const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing");
        
        out << "CSRSparseMatrix\n";
        out << rows << " " << cols << "\n";
        out << values.size() << "\n";
        
        for (const auto& v : values) out << v << " ";
        out << "\n";
        for (const auto& c : colIndices) out << c << " ";
        out << "\n";
        for (const auto& r : rowPointers) out << r << " ";
        out << "\n";
    }
    
    void loadFromFile(const std::string& filename) override {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Cannot open file for reading");
        
        std::string type;
        in >> type;
        if (type != "CSRSparseMatrix") throw std::runtime_error("Invalid file format");
        
        size_t r, c, count;
        in >> r >> c >> count;
        
        rows = r;
        cols = c;
        
        values.resize(count);
        colIndices.resize(count);
        rowPointers.resize(r + 1);
        
        for (size_t i = 0; i < count; ++i) in >> values[i];
        for (size_t i = 0; i < count; ++i) in >> colIndices[i];
        for (size_t i = 0; i < r + 1; ++i) in >> rowPointers[i];
    }
};

#endif