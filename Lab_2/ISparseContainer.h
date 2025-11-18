/**
 * @file ISparseContainer.h
 * @brief Base interface for sparse container data structures
 * @author Laboratory Work #1
 * @date 2025
 */

#ifndef ISPARSECONTAINER_H
#define ISPARSECONTAINER_H

#include <string>
#include <functional>

/**
 * @brief Base interface for sparse containers (dynamic polymorphism)
 * 
 * This interface defines the contract for sparse data structures that store
 * only non-default values, optimizing memory usage for sparse datasets.
 * 
 * @tparam T Type of elements stored in the container
 * 
 * @example
 * @code
 * SparseList<int> list(100, 0);
 * list.set(10, 42);
 * int value = list.get(10); // returns 42
 * @endcode
 */
template<typename T>
class ISparseContainer {
public:
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~ISparseContainer() = default;
    
    /**
     * @brief Get element at specified index
     * @param index Position of the element
     * @return Value at the specified index, or default value if not set
     * @throws std::out_of_range if index is out of bounds
     */
    virtual T get(size_t index) const = 0;
    
    /**
     * @brief Set element at specified index
     * @param index Position where to set the value
     * @param value Value to set at the specified position
     * @throws std::out_of_range if index is invalid
     */
    virtual void set(size_t index, const T& value) = 0;
    
    /**
     * @brief Find first occurrence of a value
     * @param value Value to search for
     * @return Index of first occurrence, or -1 if not found
     */
    virtual int findByValue(const T& value) const = 0;
    
    /**
     * @brief Find first element matching a predicate
     * @param predicate Function that returns true for matching elements
     * @return Index of first matching element, or -1 if none found
     */
    virtual int findFirstBy(std::function<bool(const T&)> predicate) const = 0;
    
    /**
     * @brief Get total size of the container
     * @return Total number of elements (including default values)
     */
    virtual size_t size() const = 0;
    
    /**
     * @brief Count non-zero (non-default) elements
     * @return Number of explicitly stored elements
     */
    virtual size_t nonZeroCount() const = 0;
    
    /**
     * @brief Get string representation of the container
     * @return String describing the container contents
     */
    virtual std::string toString() const = 0;
    
    /**
     * @brief Remove all elements from the container
     */
    virtual void clear() = 0;
    
    /**
     * @brief Save container to file
     * @param filename Path to the output file
     * @throws std::runtime_error if file cannot be opened
     */
    virtual void saveToFile(const std::string& filename) const = 0;
    
    /**
     * @brief Load container from file
     * @param filename Path to the input file
     * @throws std::runtime_error if file cannot be opened or format is invalid
     */
    virtual void loadFromFile(const std::string& filename) = 0;
};

#endif 