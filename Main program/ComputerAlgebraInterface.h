#ifndef COMPUTERALGEBRAINTERFACE_H
#define COMPUTERALGEBRAINTERFACE_H

#include "MathFunction.h"
#include <string>
#include <sstream>
#include <fstream>

class ComputerAlgebraInterface {
public:
    virtual ~ComputerAlgebraInterface() = default;
    
    virtual std::string exportToFormat(const MathFunction& func) const = 0;
    virtual void exportToFile(const MathFunction& func, const std::string& filename) const = 0;
    virtual std::string getSystemName() const = 0;
};

class MathematicaExporter : public ComputerAlgebraInterface {
public:
    std::string exportToFormat(const MathFunction& func) const override {
        std::string expr = func.toString();
        
        replaceAll(expr, "exp(", "Exp[");
        replaceAll(expr, "sin(", "Sin[");
        replaceAll(expr, "cos(", "Cos[");
        replaceAll(expr, "ln(", "Log[");
        replaceAll(expr, ")", "]");
        replaceAll(expr, "^", "^");
        
        return expr;
    }
    
    void exportToFile(const MathFunction& func, const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file");
        
        out << "(* Mathematica code *)\n";
        out << exportToFormat(func) << "\n";
        out << "\n(* Derivative *)\n";
        out << "D[" << exportToFormat(func) << ", x]\n";
        out << "\n(* Plot *)\n";
        out << "Plot[" << exportToFormat(func) << ", {x, -10, 10}]\n";
    }
    
    std::string getSystemName() const override {
        return "Mathematica";
    }
    
private:
    void replaceAll(std::string& str, const std::string& from, const std::string& to) const {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
};

class SymPyExporter : public ComputerAlgebraInterface {
public:
    std::string exportToFormat(const MathFunction& func) const override {
        std::string expr = func.toString();
        
        replaceAll(expr, "^", "**");
        replaceAll(expr, "ln(", "log(");
        
        return expr;
    }
    
    void exportToFile(const MathFunction& func, const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file");
        
        out << "# Python (SymPy) code\n";
        out << "from sympy import *\n";
        out << "x = Symbol('x')\n\n";
        out << "f = " << exportToFormat(func) << "\n";
        out << "print('Function:', f)\n";
        out << "print('Derivative:', diff(f, x))\n";
        out << "print('Integral:', integrate(f, x))\n";
        out << "\n# Plot\n";
        out << "plot(f, (x, -10, 10))\n";
    }
    
    std::string getSystemName() const override {
        return "SymPy (Python)";
    }
    
private:
    void replaceAll(std::string& str, const std::string& from, const std::string& to) const {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
};

class LaTeXExporter : public ComputerAlgebraInterface {
public:
    std::string exportToFormat(const MathFunction& func) const override {
        std::string expr = func.toString();
        
        // Базові заміни для LaTeX
        replaceAll(expr, "*", " \\cdot ");
        replaceAll(expr, "^", "^{");
        
        size_t pos = 0;
        while ((pos = expr.find("^{", pos)) != std::string::npos) {
            size_t end = expr.find_first_of(" +*)", pos + 2);
            if (end != std::string::npos && expr[end - 1] != '}') {
                expr.insert(end, "}");
            }
            pos += 2;
        }
        
        return "$" + expr + "$";
    }
    
    void exportToFile(const MathFunction& func, const std::string& filename) const override {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file");
        
        out << "\\documentclass{article}\n";
        out << "\\usepackage{amsmath}\n";
        out << "\\begin{document}\n\n";
        out << "Function: " << exportToFormat(func) << "\n\n";
        out << "Derivative: " << exportToFormat(func.derivative()) << "\n\n";
        out << "\\end{document}\n";
    }
    
    std::string getSystemName() const override {
        return "LaTeX";
    }
    
private:
    void replaceAll(std::string& str, const std::string& from, const std::string& to) const {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
};

class CASystemManager {
private:
    std::vector<std::shared_ptr<ComputerAlgebraInterface>> exporters;
    
public:
    CASystemManager() {
        exporters.push_back(std::make_shared<MathematicaExporter>());
        exporters.push_back(std::make_shared<SymPyExporter>());
        exporters.push_back(std::make_shared<LaTeXExporter>());
    }
    
    void exportToAll(const MathFunction& func, const std::string& baseFilename) const {
        for (const auto& exporter : exporters) {
            std::string filename = baseFilename + "_" + exporter->getSystemName();
            std::cout << "Exporting to " << exporter->getSystemName() << "...\n";
            exporter->exportToFile(func, filename);
        }
    }
    
    void exportTo(const MathFunction& func, const std::string& filename, size_t exporterIndex) const {
        if (exporterIndex >= exporters.size()) {
            throw std::out_of_range("Invalid exporter index");
        }
        exporters[exporterIndex]->exportToFile(func, filename);
    }
    
    void listAvailableSystems() const {
        std::cout << "Available export systems:\n";
        for (size_t i = 0; i < exporters.size(); ++i) {
            std::cout << i << ". " << exporters[i]->getSystemName() << "\n";
        }
    }
};

#endif