#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include "MathExpression.h"
#include <vector>
#include <fstream>

class MathFunction {
private:
    std::shared_ptr<MathExpression> expression;
    std::string name;
    
public:
    MathFunction(std::shared_ptr<MathExpression> expr, const std::string& n = "f")
        : expression(expr), name(n) {}
    
    double evaluate(double x) const {
        return expression->evaluate(x);
    }
    
    std::string toString() const {
        return name + "(x) = " + expression->toString();
    }
    
    MathFunction derivative() const {
        return MathFunction(expression->derivative(), name + "'");
    }
    
    MathFunction nthDerivative(int n) const {
        if (n < 0) throw std::invalid_argument("Derivative order must be non-negative");
        if (n == 0) return MathFunction(expression->clone(), name);
        
        auto result = expression->derivative();
        for (int i = 1; i < n; ++i) {
            result = result->derivative();
        }
        
        std::string newName = name;
        for (int i = 0; i < n; ++i) newName += "'";
        
        return MathFunction(result, newName);
    }
    
    double integrate(double a, double b, int steps = 1000) const {
        if (steps <= 0) throw std::invalid_argument("Steps must be positive");
        
        double h = (b - a) / steps;
        double sum = 0.5 * (evaluate(a) + evaluate(b));
        
        for (int i = 1; i < steps; ++i) {
            sum += evaluate(a + i * h);
        }
        
        return sum * h;
    }
    
    double limit(double point, double epsilon = 1e-6) const {
        return evaluate(point + epsilon);
    }
    
    std::vector<double> taylorSeries(double point, int terms) const {
        std::vector<double> coefficients;
        MathFunction current(expression->clone(), name);
        double factorial = 1.0;
        
        for (int i = 0; i < terms; ++i) {
            if (i > 0) factorial *= i;
            double coef = current.evaluate(point) / factorial;
            coefficients.push_back(coef);
            
            if (i < terms - 1) {
                current = current.derivative();
            }
        }
        
        return coefficients;
    }
    
    double seriesSum(int start, int end, std::function<double(int)> termFunction) const {
        double sum = 0.0;
        for (int n = start; n <= end; ++n) {
            sum += termFunction(n);
        }
        return sum;
    }
    
    double findRoot(double initialGuess, double tolerance = 1e-6, int maxIterations = 100) const {
        auto deriv = derivative();
        double x = initialGuess;
        
        for (int i = 0; i < maxIterations; ++i) {
            double fx = evaluate(x);
            double dfx = deriv.evaluate(x);
            
            if (std::abs(dfx) < tolerance) {
                throw std::runtime_error("Derivative too small");
            }
            
            double xNew = x - fx / dfx;
            
            if (std::abs(xNew - x) < tolerance) {
                return xNew;
            }
            
            x = xNew;
        }
        
        throw std::runtime_error("Root finding did not converge");
    }
    
    std::vector<std::pair<double, double>> tabulate(double start, double end, int points) const {
        std::vector<std::pair<double, double>> result;
        double step = (end - start) / (points - 1);
        
        for (int i = 0; i < points; ++i) {
            double x = start + i * step;
            result.push_back({x, evaluate(x)});
        }
        
        return result;
    }
    
    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing");
        
        out << "MathFunction\n";
        out << name << "\n";
        out << expression->toString() << "\n";
    }
    
    void exportTabulatedData(const std::string& filename, double start, double end, int points) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for writing");
        
        auto data = tabulate(start, end, points);
        out << "x\t" << name << "(x)\n";
        for (const auto& point : data) {
            out << point.first << "\t" << point.second << "\n";
        }
    }
};

#endif