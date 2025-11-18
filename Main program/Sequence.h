#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>

class Sequence {
protected:
    std::string name;
    
public:
    Sequence(const std::string& n = "a") : name(n) {}
    virtual ~Sequence() = default;
    
    virtual double getTerm(int n) const = 0;
    virtual std::string toString() const = 0;
    
    std::vector<double> generateTerms(int start, int count) const {
        std::vector<double> terms;
        for (int i = 0; i < count; ++i) {
            terms.push_back(getTerm(start + i));
        }
        return terms;
    }
    
    double partialSum(int start, int end) const {
        double sum = 0.0;
        for (int i = start; i <= end; ++i) {
            sum += getTerm(i);
        }
        return sum;
    }
    
    bool checkConvergence(int testTerms = 1000, double tolerance = 1e-6) const {
        double term = getTerm(testTerms);
        return std::abs(term) < tolerance;
    }
    
    double computeLimit(int maxTerms = 10000, double tolerance = 1e-6) const {
        double prev = getTerm(1);
        for (int n = 2; n <= maxTerms; ++n) {
            double curr = getTerm(n);
            if (std::abs(curr - prev) < tolerance) {
                return curr;
            }
            prev = curr;
        }
        throw std::runtime_error("Limit did not converge");
    }
    
    void saveToFile(const std::string& filename, int start, int count) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file");
        
        out << "Sequence: " << name << "\n";
        out << "n\t" << name << "(n)\n";
        for (int i = 0; i < count; ++i) {
            int n = start + i;
            out << n << "\t" << getTerm(n) << "\n";
        }
    }
};

class ArithmeticSequence : public Sequence {
private:
    double firstTerm;
    double difference;
    
public:
    ArithmeticSequence(double a1, double d, const std::string& n = "a")
        : Sequence(n), firstTerm(a1), difference(d) {}
    
    double getTerm(int n) const override {
        return firstTerm + (n - 1) * difference;
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << name << "(n) = " << firstTerm << " + " << difference << "*(n-1)";
        return oss.str();
    }
};

class GeometricSequence : public Sequence {
private:
    double firstTerm;
    double ratio;
    
public:
    GeometricSequence(double a1, double r, const std::string& n = "g")
        : Sequence(n), firstTerm(a1), ratio(r) {}
    
    double getTerm(int n) const override {
        return firstTerm * std::pow(ratio, n - 1);
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << name << "(n) = " << firstTerm << " * " << ratio << "^(n-1)";
        return oss.str();
    }
};

class RecursiveSequence : public Sequence {
private:
    std::vector<double> initialTerms;
    std::function<double(const std::vector<double>&)> recurrenceRelation;
    mutable std::vector<double> cache;
    
public:
    RecursiveSequence(const std::vector<double>& initial,
                     std::function<double(const std::vector<double>&)> relation,
                     const std::string& n = "f")
        : Sequence(n), initialTerms(initial), recurrenceRelation(relation), cache(initial) {}
    
    double getTerm(int n) const override {
        if (n < 1) throw std::invalid_argument("Term index must be positive");
        
        while (cache.size() < static_cast<size_t>(n)) {
            std::vector<double> previous;
            size_t start = cache.size() - initialTerms.size();
            for (size_t i = start; i < cache.size(); ++i) {
                previous.push_back(cache[i]);
            }
            cache.push_back(recurrenceRelation(previous));
        }
        
        return cache[n - 1];
    }
    
    std::string toString() const override {
        return name + "(n) = recurrence relation";
    }
};

class FunctionalSequence : public Sequence {
private:
    std::function<double(int)> termFunction;
    std::string formula;
    
public:
    FunctionalSequence(std::function<double(int)> func, const std::string& form, const std::string& n = "s")
        : Sequence(n), termFunction(func), formula(form) {}
    
    double getTerm(int n) const override {
        return termFunction(n);
    }
    
    std::string toString() const override {
        return name + "(n) = " + formula;
    }
};

#endif