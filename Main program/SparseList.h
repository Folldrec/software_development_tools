/**
 * @file SparseList.h
 * @brief Implementation of sparse list data structure
 */

#ifndef SPARSELIST_H
#define SPARSELIST_H

#include "ISparseContainer.h"
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>

/**
 * @brief Sparse list implementation that stores only non-default values
 * 
 * This class implements a sparse list using std::map to store only
 * non-default values, providing efficient memory usage for sparse datasets.
 * 
 * @tparam T Type of elements stored in the list
 * 
 * @example
 * @code
 * // Create a sparse list of 1000 integers with default value 0
 * SparseList<int> list(1000, 0);
 * 
 * 
 * list.set(10, 42);
 * list.set(500, 99);
 * 
 *
 * std::cout << "Non-zero count: " << list.nonZeroCount() << std::endl; // Output: 2
 * @endcode
 */
template<typename T>
class SparseList : public ISparseContainer<T> {
private:
    std::map<size_t, T> data;     
    size_t listSize;               
    T defaultValue;                
public:
    /**
     * @brief Construct a sparse list
     * @param size Initial size of the list
     * @param defVal Default value for unset elements
     */
    SparseList(size_t size = 0, const T& defVal = T()) 
        : listSize(size), defaultValue(defVal) {}
    
    /**
     * @brief Get element at specified index
     * @param index Position of the element
     * @return Value at index, or default value if not explicitly set
     * @throws std::out_of_range if index >= listSize
     */
    T get(size_t index) const override {
        if (index >= listSize) {
            throw std::out_of_range("Index out of range");
        }
        auto it = data.find(index);
        return (it != data.end()) ? it->second : defaultValue;
    }
    
    void set(size_t index, const T& value) override {
        if (index >= listSize) {
            listSize = index + 1;
        }
        
        if (value == defaultValue) {
            data.erase(index);
        } else {
            data[index] = value;
        }
    }
    
    int findByValue(const T& value) const override {
        if (value == defaultValue) {
            for (size_t i = 0; i < listSize; ++i) {
                if (data.find(i) == data.end()) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }
        
        for (const auto& pair : data) {
            if (pair.second == value) {
                return static_cast<int>(pair.first);
            }
        }
        return -1;
    }
    
    int findFirstBy(std::function<bool(const T&)> predicate) const override {
        for (size_t i = 0; i < listSize; ++i) {
            T val = get(i);
            if (predicate(val)) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    size_t size() const override {
        return listSize;
    }
    
    size_t nonZeroCount() const override {
        return data.size();
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << "SparseList[size=" << listSize << ", stored=" << data.size() << "]: [";
        for (size_t i = 0; i < std::min(listSize, size_t(10)); ++i) {
            if (i > 0) oss << ", ";
            oss << get(i);
        }
        if (listSize > 10) oss << ", ...";
        oss << "]";
        return oss.str();
    }
    
    void clear() override {
        data.clear();
        listSize = 0;
    }
    
    void saveToFile(const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing");
        
        out << "SparseList\n";
        out << listSize << "\n";
        out << data.size() << "\n";
        for (const auto& pair : data) {
            out << pair.first << " " << pair.second << "\n";
        }
    }
    
    void loadFromFile(const std::string& filename) override {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Cannot open file for reading");
        
        std::string type;
        in >> type;
        if (type != "SparseList") throw std::runtime_error("Invalid file format");
        
        size_t sz, count;
        in >> sz >> count;
        
        clear();
        listSize = sz;
        for (size_t i = 0; i < count; ++i) {
            size_t idx;
            T val;
            in >> idx >> val;
            data[idx] = val;
        }
    }
    
    void generateRandom(size_t size, double density, std::function<T()> generator) {
        clear();
        listSize = size;
        size_t count = static_cast<size_t>(size * density);
        for (size_t i = 0; i < count; ++i) {
            size_t idx = rand() % size;
            data[idx] = generator();
        }
    }
};

#endif