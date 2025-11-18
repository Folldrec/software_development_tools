#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <type_traits>
#include "ISparseContainer.h"
#include "SparseList.h"
#include "SparseMatrix.h"
#include "MathExpression.h"
#include "MathFunction.h"
#include "Sequence.h"
#include "ComputerAlgebraInterface.h"

using namespace std;

template<typename T>
void demonstrateContainer(ISparseContainer<T>& container, const string& containerName) {
    cout << "\n=== Testing " << containerName << " ===\n";
    cout << container.toString() << "\n";
    cout << "Size: " << container.size() << "\n";
    cout << "Non-zero elements: " << container.nonZeroCount() << "\n";
    
    if (container.size() > 0) {
        T firstElem = container.get(0);
        int foundIndex = container.findByValue(firstElem);
        cout << "First element found at index: " << foundIndex << "\n";
    }
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
demonstrateContainerNumeric(ISparseContainer<T>& container, const string& containerName) {
    demonstrateContainer(container, containerName);
    
    if (container.size() > 0) {
        int largeIndex = container.findFirstBy([](const T& val) { return val > T(5); });
        if (largeIndex >= 0) {
            cout << "First element > 5 at index: " << largeIndex << "\n";
        } else {
            cout << "No element > 5 found\n";
        }
    }
}

void demonstrateContainerString(ISparseContainer<string>& container, const string& containerName) {
    demonstrateContainer(container, containerName);
    
    if (container.size() > 0) {
        int longIndex = container.findFirstBy([](const string& val) { return val.length() > 3; });
        if (longIndex >= 0) {
            cout << "First string with length > 3 at index: " << longIndex << "\n";
        } else {
            cout << "No string with length > 3 found\n";
        }
    }
}

void demonstrateSparseStructures() {
    
    SparseList<int> intList(100, 0);
    intList.generateRandom(100, 0.1, []() { return rand() % 20 + 1; });
    demonstrateContainerNumeric(intList, "Sparse List (int)");
    
    intList.saveToFile("sparse_list_int.txt");
    cout << "Saved to file: sparse_list_int.txt\n";
    
    SparseList<int> loadedIntList;
    loadedIntList.loadFromFile("sparse_list_int.txt");
    cout << "Loaded from file successfully\n";
    
    SparseList<double> doubleList(50, 0.0);
    doubleList.generateRandom(50, 0.15, []() { return (rand() % 1000) / 100.0; });
    demonstrateContainerNumeric(doubleList, "Sparse List (double)");
    
    
    MapSparseMatrix<int> matrix1(10, 10, 0);
    matrix1.generateRandom(10, 10, 0.2, []() { return rand() % 10 + 1; });
    cout << "\nMatrix 1:\n" << matrix1.toString();
    
    MapSparseMatrix<int> matrix2(10, 10, 0);
    matrix2.generateRandom(10, 10, 0.2, []() { return rand() % 10 + 1; });
    cout << "\nMatrix 2:\n" << matrix2.toString();
    
    cout << "\n--- Matrix Addition ---\n";
    auto sumMatrix = unique_ptr<SparseMatrix<int>>(matrix1.add(matrix2));
    cout << sumMatrix->toString();
    
    cout << "\n--- Matrix Multiplication ---\n";
    auto prodMatrix = unique_ptr<SparseMatrix<int>>(matrix1.multiply(matrix2));
    cout << prodMatrix->toString();
    
    cout << "\n--- Matrix Transpose ---\n";
    auto transpMatrix = unique_ptr<SparseMatrix<int>>(matrix1.transpose());
    cout << transpMatrix->toString();
    
    cout << "\n--- Matrix-Vector Multiplication ---\n";
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto resultVec = matrix1.multiplyVector(vec);
    cout << "Result vector: [";
    for (size_t i = 0; i < min(resultVec.size(), size_t(10)); ++i) {
        if (i > 0) cout << ", ";
        cout << resultVec[i];
    }
    cout << "]\n";
    
    matrix1.saveToFile("sparse_matrix.txt");
    cout << "\nMatrix saved to: sparse_matrix.txt\n";
    
    cout << "\n=== Efficiency Comparison ===\n";
    cout << "MapSparseMatrix storage: " << matrix1.nonZeroCount() << " elements\n";
    cout << "Total elements: " << matrix1.getRows() * matrix1.getCols() << "\n";
    cout << "Density: " << (100.0 * matrix1.nonZeroCount() / (matrix1.getRows() * matrix1.getCols())) << "%\n";
}

void demonstrateMathematicalAnalysis() {
        
    auto x = make_shared<Variable>();
    auto x2 = make_shared<Power>(x, 2);
    auto two = make_shared<Constant>(2);
    auto twoX = make_shared<Product>(two, x);
    auto one = make_shared<Constant>(1);
    auto sum1 = make_shared<Sum>(x2, twoX);
    auto polynomial = make_shared<Sum>(sum1, one);
    
    MathFunction polyFunc(polynomial, "f");
    cout << polyFunc.toString() << "\n";
    cout << "f(3) = " << polyFunc.evaluate(3) << "\n";
    
    cout << "\n--- Derivative ---\n";
    auto polyDeriv = polyFunc.derivative();
    cout << polyDeriv.toString() << "\n";
    cout << "f'(3) = " << polyDeriv.evaluate(3) << "\n";
    
    cout << "\n--- Second Derivative ---\n";
    auto polyDeriv2 = polyFunc.nthDerivative(2);
    cout << polyDeriv2.toString() << "\n";
    cout << "f''(3) = " << polyDeriv2.evaluate(3) << "\n";
    
    cout << "\n--- Integration ---\n";
    double integral = polyFunc.integrate(0, 1, 1000);
    cout << "Integral from 0 to 1: " << integral << "\n";
    
    auto sinFunc = make_shared<Sin>(x);
    MathFunction sinMath(sinFunc, "g");
    cout << sinMath.toString() << "\n";
    cout << "g(π/2) ≈ g(1.5708) = " << sinMath.evaluate(1.5708) << "\n";
    
    auto sinDeriv = sinMath.derivative();
    cout << sinDeriv.toString() << "\n";
    cout << "g'(0) = " << sinDeriv.evaluate(0) << "\n";
    
    cout << "\n=== Exponential Function ===\n";
    auto expFunc = make_shared<Exp>(x);
    MathFunction expMath(expFunc, "h");
    cout << expMath.toString() << "\n";
    cout << "h(1) = " << expMath.evaluate(1) << " (should be ≈ 2.718)\n";
    
    auto expDeriv = expMath.derivative();
    cout << expDeriv.toString() << "\n";
    cout << "h'(1) = " << expDeriv.evaluate(1) << "\n";
    
    cout << "\n--- Taylor Series ---\n";
    auto taylorCoefs = expMath.taylorSeries(0, 6);
    cout << "Taylor series coefficients for e^x at x=0:\n";
    for (size_t i = 0; i < taylorCoefs.size(); ++i) {
        cout << "  a" << i << " = " << taylorCoefs[i] << "\n";
    }
    
    cout << "\n--- Root Finding ---\n";
    try {
        auto x2minus4 = make_shared<Sum>(
            make_shared<Power>(x, 2),
            make_shared<Constant>(-4)
        );
        MathFunction rootFunc(x2minus4, "r");
        cout << rootFunc.toString() << "\n";
        double root = rootFunc.findRoot(3.0);
        cout << "Root found: " << root << " (expected ≈ 2.0)\n";
    } catch (const exception& e) {
        cout << "Root finding error: " << e.what() << "\n";
    }
    
    cout << "\n--- Function Tabulation ---\n";
    polyFunc.exportTabulatedData("polynomial_data.txt", -2, 2, 20);
    cout << "Tabulated data saved to: polynomial_data.txt\n";
}

void demonstrateSequences() {
    
    ArithmeticSequence arith(1, 2, "a");
    cout << arith.toString() << "\n";
    auto arithTerms = arith.generateTerms(1, 10);
    cout << "First 10 terms: ";
    for (auto term : arithTerms) cout << term << " ";
    cout << "\n";
    cout << "Partial sum (1 to 10): " << arith.partialSum(1, 10) << "\n";
    
    cout << "\n=== Geometric Sequence ===\n";
    GeometricSequence geom(2, 0.5, "g");
    cout << geom.toString() << "\n";
    auto geomTerms = geom.generateTerms(1, 10);
    cout << "First 10 terms: ";
    for (auto term : geomTerms) cout << term << " ";
    cout << "\n";
    cout << "Converges: " << (geom.checkConvergence() ? "Yes" : "No") << "\n";
    
    cout << "\n=== Fibonacci Sequence ===\n";
    RecursiveSequence fib(
        {1, 1},
        [](const vector<double>& prev) { return prev[0] + prev[1]; },
        "F"
    );
    cout << fib.toString() << "\n";
    auto fibTerms = fib.generateTerms(1, 15);
    cout << "First 15 terms: ";
    for (auto term : fibTerms) cout << term << " ";
    cout << "\n";
    
    cout << "\n=== Functional Sequence ===\n";
    FunctionalSequence harmonic(
        [](int n) { return 1.0 / n; },
        "1/n",
        "H"
    );
    cout << harmonic.toString() << "\n";
    cout << "Partial sum (1 to 100): " << harmonic.partialSum(1, 100) << "\n";
    
    arith.saveToFile("arithmetic_sequence.txt", 1, 20);
    cout << "\nArithmetic sequence saved to: arithmetic_sequence.txt\n";
}

void demonstrateComputerAlgebraIntegration() {
    
    auto x = make_shared<Variable>();
    auto x2 = make_shared<Power>(x, 2);
    auto sinx = make_shared<Sin>(x);
    auto expr = make_shared<Sum>(x2, sinx);
    
    MathFunction func(expr, "f");
    cout << "\nFunction: " << func.toString() << "\n";
    
    CASystemManager manager;
    cout << "\n";
    manager.listAvailableSystems();
    
    cout << "\nExporting function to various formats...\n";
    
    MathematicaExporter mathematica;
    cout << "\nMathematica format:\n" << mathematica.exportToFormat(func) << "\n";
    mathematica.exportToFile(func, "function_mathematica.m");
    cout << "Saved to: function_mathematica.m\n";
    
    SymPyExporter sympy;
    cout << "\nSymPy format:\n" << sympy.exportToFormat(func) << "\n";
    sympy.exportToFile(func, "function_sympy.py");
    cout << "Saved to: function_sympy.py\n";
    
    LaTeXExporter latex;
    cout << "\nLaTeX format:\n" << latex.exportToFormat(func) << "\n";
    latex.exportToFile(func, "function_latex.tex");
    cout << "Saved to: function_latex.tex\n";
    
    cout << "\nAll export files created successfully!\n";
}

void demonstratePolymorphism() {
    
    cout << "\n=== Static Polymorphism (Templates) ===\n";
    cout << "Working with different types through template interface:\n";
    
    SparseList<int> intList(20, 0);
    intList.generateRandom(20, 0.3, []() { return rand() % 50; });
    demonstrateContainerNumeric<int>(intList, "Integer List");
    
    SparseList<double> doubleList(20, 0.0);
    doubleList.generateRandom(20, 0.3, []() { return (rand() % 1000) / 10.0; });
    demonstrateContainerNumeric<double>(doubleList, "Double List");
    
    SparseList<string> stringList(10, "");
    stringList.set(2, "Hello");
    stringList.set(5, "World");
    stringList.set(8, "C++");
    demonstrateContainerString(stringList, "String List");
    
    cout << "\n=== Dynamic Polymorphism (Virtual Methods) ===\n";
    cout << "Working with mathematical expressions through base class pointer:\n";
    
    vector<shared_ptr<MathExpression>> expressions;
    expressions.push_back(make_shared<Constant>(5));
    expressions.push_back(make_shared<Variable>());
    expressions.push_back(make_shared<Sin>(make_shared<Variable>()));
    expressions.push_back(make_shared<Power>(make_shared<Variable>(), 2));
    
    double testValue = 2.0;
    cout << "Evaluating expressions at x = " << testValue << ":\n";
    for (size_t i = 0; i < expressions.size(); ++i) {
        cout << "  " << expressions[i]->toString() 
             << " = " << expressions[i]->evaluate(testValue) << "\n";
    }
    
    cout << "\nComputing derivatives (runtime polymorphism):\n";
    for (size_t i = 0; i < expressions.size(); ++i) {
        auto deriv = expressions[i]->derivative();
        cout << "  d/dx[" << expressions[i]->toString() << "] = " 
             << deriv->toString() << "\n";
    }
    
    cout << "\n=== Dynamic Polymorphism (Export Systems) ===\n";
    auto func = MathFunction(make_shared<Power>(make_shared<Variable>(), 3), "f");
    
    vector<shared_ptr<ComputerAlgebraInterface>> exporters;
    exporters.push_back(make_shared<MathematicaExporter>());
    exporters.push_back(make_shared<SymPyExporter>());
    exporters.push_back(make_shared<LaTeXExporter>());
    
    cout << "Exporting " << func.toString() << " to different formats:\n";
    for (const auto& exporter : exporters) {
        cout << "  " << exporter->getSystemName() << ": " 
             << exporter->exportToFormat(func) << "\n";
    }
}

void workWithSparseLists() {
    
    cout << "\nSelect data type:\n";
    cout << "1. int\n";
    cout << "2. double\n";
    cout << "3. string\n";
    cout << "Your choice: ";
    
    int typeChoice;
    cin >> typeChoice;
    
    if (typeChoice == 1 || typeChoice == 2) {
        cout << "Enter list size: ";
        size_t size;
        cin >> size;
        
        cout << "Enter density (0.0-1.0): ";
        double density;
        cin >> density;
        
        if (typeChoice == 1) {
            SparseList<int> list(size, 0);
            list.generateRandom(size, density, []() { return rand() % 100; });
            cout << "\n" << list.toString() << "\n";
            
            cout << "\nSave to file? (y/n): ";
            char save;
            cin >> save;
            if (save == 'y' || save == 'Y') {
                list.saveToFile("my_sparse_list_int.txt");
                cout << "Saved to: my_sparse_list_int.txt\n";
            }
        } else {
            SparseList<double> list(size, 0.0);
            list.generateRandom(size, density, []() { return (rand() % 10000) / 100.0; });
            cout << "\n" << list.toString() << "\n";
            
            cout << "\nSave to file? (y/n): ";
            char save;
            cin >> save;
            if (save == 'y' || save == 'Y') {
                list.saveToFile("my_sparse_list_double.txt");
                cout << "Saved to: my_sparse_list_double.txt\n";
            }
        }
    } else if (typeChoice == 3) {
        cout << "Enter list size: ";
        size_t size;
        cin >> size;
        
        SparseList<string> list(size, "");
        
        cout << "How many elements to fill? ";
        int count;
        cin >> count;
        
        for (int i = 0; i < count; ++i) {
            cout << "Index: ";
            size_t idx;
            cin >> idx;
            cout << "Value: ";
            string val;
            cin >> val;
            list.set(idx, val);
        }
        
        cout << "\n" << list.toString() << "\n";
    }
}

void workWithSparseMatrices() {
    
    cout << "Enter number of rows: ";
    size_t rows;
    cin >> rows;
    
    cout << "Enter number of columns: ";
    size_t cols;
    cin >> cols;
    
    cout << "Enter density (0.0-1.0): ";
    double density;
    cin >> density;
    
    MapSparseMatrix<int> matrix(rows, cols, 0);
    matrix.generateRandom(rows, cols, density, []() { return rand() % 20 + 1; });
    
    cout << "\nCreated matrix:\n" << matrix.toString();
    
    cout << "\nOperations:\n";
    cout << "1. Transpose\n";
    cout << "2. Multiply by vector\n";
    cout << "3. Save to file\n";
    cout << "0. Return\n";
    cout << "Your choice: ";
    
    int opChoice;
    cin >> opChoice;
    
    if (opChoice == 1) {
        auto transposed = unique_ptr<SparseMatrix<int>>(matrix.transpose());
        cout << "\nTransposed matrix:\n" << transposed->toString();
    } else if (opChoice == 2) {
        vector<int> vec(cols);
        cout << "Enter " << cols << " vector elements:\n";
        for (size_t i = 0; i < cols; ++i) {
            cout << "vec[" << i << "] = ";
            cin >> vec[i];
        }
        
        auto result = matrix.multiplyVector(vec);
        cout << "\nResult: [";
        for (size_t i = 0; i < result.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << result[i];
        }
        cout << "]\n";
    } else if (opChoice == 3) {
        matrix.saveToFile("my_matrix.txt");
        cout << "Saved to: my_matrix.txt\n";
    }
}

void workWithMathFunctions() {
    
    cout << "\nSelect function:\n";
    cout << "1. Polynomial (x^2 + 2x + 1)\n";
    cout << "2. Sin(x)\n";
    cout << "3. Cos(x)\n";
    cout << "4. Exp(x)\n";
    cout << "5. Ln(x)\n";
    cout << "6. x^3\n";
    cout << "Your choice: ";
    
    int choice;
    cin >> choice;
    
    shared_ptr<MathExpression> expr;
    auto x = make_shared<Variable>();
    
    if (choice == 1) {
        auto x2 = make_shared<Power>(x, 2);
        auto two = make_shared<Constant>(2);
        auto twoX = make_shared<Product>(two, x);
        auto one = make_shared<Constant>(1);
        auto sum1 = make_shared<Sum>(x2, twoX);
        expr = make_shared<Sum>(sum1, one);
    } else if (choice == 2) {
        expr = make_shared<Sin>(x);
    } else if (choice == 3) {
        expr = make_shared<Cos>(x);
    } else if (choice == 4) {
        expr = make_shared<Exp>(x);
    } else if (choice == 5) {
        expr = make_shared<Ln>(x);
    } else if (choice == 6) {
        expr = make_shared<Power>(x, 3);
    } else {
        cout << "Invalid choice!\n";
        return;
    }
    
    MathFunction func(expr, "f");
    cout << "\n" << func.toString() << "\n";
    
    while (true) {
        cout << "\nOperations:\n";
        cout << "1. Evaluate\n";
        cout << "2. Compute derivative\n";
        cout << "3. Compute integral\n";
        cout << "4. Taylor series expansion\n";
        cout << "5. Find root\n";
        cout << "6. Tabulate\n";
        cout << "7. Export to CAS\n";
        cout << "0. Return\n";
        cout << "Your choice: ";
        
        int op;
        cin >> op;
        
        if (op == 0) {
            break;
        }
        
        if (op == 1) {
            cout << "Enter x: ";
            double xVal;
            cin >> xVal;
            cout << "f(" << xVal << ") = " << func.evaluate(xVal) << "\n";
        } else if (op == 2) {
            auto deriv = func.derivative();
            cout << deriv.toString() << "\n";
            cout << "Evaluate at point? (y/n): ";
            char yn;
            cin >> yn;
            if (yn == 'y' || yn == 'Y') {
                cout << "Enter x: ";
                double xVal;
                cin >> xVal;
                cout << "f'(" << xVal << ") = " << deriv.evaluate(xVal) << "\n";
            }
        } else if (op == 3) {
            cout << "From: ";
            double a;
            cin >> a;
            cout << "To: ";
            double b;
            cin >> b;
            cout << "Integral = " << func.integrate(a, b) << "\n";
        } else if (op == 4) {
            cout << "Expansion point: ";
            double point;
            cin >> point;
            cout << "Number of terms: ";
            int terms;
            cin >> terms;
            
            auto coefs = func.taylorSeries(point, terms);
            cout << "Taylor series coefficients:\n";
            for (size_t i = 0; i < coefs.size(); ++i) {
                cout << "  a" << i << " = " << coefs[i] << "\n";
            }
        } else if (op == 5) {
            cout << "Initial guess: ";
            double guess;
            cin >> guess;
            try {
                double root = func.findRoot(guess);
                cout << "Found root: " << root << "\n";
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        } else if (op == 6) {
            cout << "From: ";
            double start;
            cin >> start;
            cout << "To: ";
            double end;
            cin >> end;
            cout << "Number of points: ";
            int points;
            cin >> points;
            
            string filename = "tabulated_function.txt";
            func.exportTabulatedData(filename, start, end, points);
            cout << "Saved to: " << filename << "\n";
        } else if (op == 7) {
            CASystemManager manager;
            cout << "\nExporting to formats:\n";
            func.saveToFile("my_function.txt");
            cout << "  - my_function.txt (internal format)\n";
            
            MathematicaExporter mathematica;
            mathematica.exportToFile(func, "my_function_mathematica.m");
            cout << "  - my_function_mathematica.m\n";
            
            SymPyExporter sympy;
            sympy.exportToFile(func, "my_function_sympy.py");
            cout << "  - my_function_sympy.py\n";
            
            LaTeXExporter latex;
            latex.exportToFile(func, "my_function_latex.tex");
            cout << "  - my_function_latex.tex\n";
        }
    }
}

void workWithSequences() {
  
    cout << "\nSelect sequence type:\n";
    cout << "1. Arithmetic progression\n";
    cout << "2. Geometric progression\n";
    cout << "3. Fibonacci\n";
    cout << "4. 1/n (harmonic)\n";
    cout << "Your choice: ";
    
    int choice;
    cin >> choice;
    
    shared_ptr<Sequence> seq;
    
    if (choice == 1) {
        cout << "First term: ";
        double a1;
        cin >> a1;
        cout << "Common difference: ";
        double d;
        cin >> d;
        seq = make_shared<ArithmeticSequence>(a1, d, "a");
    } else if (choice == 2) {
        cout << "First term: ";
        double a1;
        cin >> a1;
        cout << "Common ratio: ";
        double r;
        cin >> r;
        seq = make_shared<GeometricSequence>(a1, r, "g");
    } else if (choice == 3) {
        seq = make_shared<RecursiveSequence>(
            vector<double>{1, 1},
            [](const vector<double>& prev) { return prev[0] + prev[1]; },
            "F"
        );
    } else if (choice == 4) {
        seq = make_shared<FunctionalSequence>(
            [](int n) { return 1.0 / n; },
            "1/n",
            "H"
        );
    } else {
        cout << "Invalid choice!\n";
        return;
    }
    
    cout << "\n" << seq->toString() << "\n";
    
    cout << "\nHow many terms to display? ";
    int count;
    cin >> count;
    
    auto terms = seq->generateTerms(1, count);
    cout << "First " << count << " terms: ";
    for (int i = 0; i < count; ++i) {
        if (i > 0) cout << ", ";
        cout << terms[i];
    }
    cout << "\n";
    
    cout << "\nCompute partial sum (1 to " << count << ")? (y/n): ";
    char yn;
    cin >> yn;
    if (yn == 'y' || yn == 'Y') {
        cout << "Sum = " << seq->partialSum(1, count) << "\n";
    }
    
    cout << "\nSave to file? (y/n): ";
    cin >> yn;
    if (yn == 'y' || yn == 'Y') {
        seq->saveToFile("my_sequence.txt", 1, count);
        cout << "Saved to: my_sequence.txt\n";
    }
}

void runAllDemonstrations() {
    
    demonstrateSparseStructures();
    demonstrateMathematicalAnalysis();
    demonstrateSequences();
    demonstrateComputerAlgebraIntegration();
    demonstratePolymorphism();
    
    
    cout << "\nGenerated files:\n";
    cout << "  - sparse_list_int.txt\n";
    cout << "  - sparse_matrix.txt\n";
    cout << "  - polynomial_data.txt\n";
    cout << "  - arithmetic_sequence.txt\n";
    cout << "  - function_mathematica.m\n";
    cout << "  - function_sympy.py\n";
    cout << "  - function_latex.tex\n";
}

void interactiveMenu() {
    while (true) {
        cout << "MAIN MENU:\n";
        cout << "1. Work with Sparse Lists\n";
        cout << "2. Work with Sparse Matrices\n";
        cout << "3. Mathematical Analysis\n";
        cout << "4. Sequences\n";
        cout << "5. Demonstrate Polymorphism\n";
        cout << "6. Run All Demonstrations\n";
        cout << "0. Exit\n";
        cout << "Select option: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            workWithSparseLists();
        } else if (choice == 2) {
            workWithSparseMatrices();
        } else if (choice == 3) {
            workWithMathFunctions();
        } else if (choice == 4) {
            workWithSequences();
        } else if (choice == 5) {
            demonstratePolymorphism();
        } else if (choice == 6) {
            runAllDemonstrations();
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    try {
        
        interactiveMenu();
        
    } catch (const exception& e) {
        cerr << "\nError: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}