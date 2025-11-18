#ifndef ISPARSECONTAINER_H
#define ISPARSECONTAINER_H

#include <string>
#include <functional>

template<typename T>
class ISparseContainer {
public:
    virtual ~ISparseContainer() = default;
    
    virtual T get(size_t index) const = 0;
    virtual void set(size_t index, const T& value) = 0;
    
    virtual int findByValue(const T& value) const = 0;
    virtual int findFirstBy(std::function<bool(const T&)> predicate) const = 0;
    
    virtual size_t size() const = 0;
    virtual size_t nonZeroCount() const = 0;
    virtual std::string toString() const = 0;
    virtual void clear() = 0;
    
    virtual void saveToFile(const std::string& filename) const = 0;
    virtual void loadFromFile(const std::string& filename) = 0;
};

#endif