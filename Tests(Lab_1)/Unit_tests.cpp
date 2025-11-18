#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include "ISparseContainer.h"
#include "SparseList.h"
#include "SparseMatrix.h"
#include "MathExpression.h"
#include "MathFunction.h"
#include "Sequence.h"

using namespace std;

int testsPassed = 0;
int testsFailed = 0;

void reportTest(const string& testName, bool passed) {
    if (passed) {
        cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        cout << "[FAIL] " << testName << "\n";
        testsFailed++;
    }
}

void test_SparseList_SetGet() {
    SparseList<int> list(10, 0);
    list.set(3, 42);
    list.set(7, 100);
    
    bool passed = (list.get(3) == 42) && (list.get(7) == 100) && (list.get(0) == 0);
    reportTest("SparseList - Set and Get", passed);
}

void test_SparseList_SizeCount() {
    SparseList<int> list(20, 0);
    list.set(5, 10);
    list.set(10, 20);
    list.set(15, 30);
    
    bool passed = (list.size() == 20) && (list.nonZeroCount() == 3);
    reportTest("SparseList - Size and NonZeroCount", passed);
}

void test_SparseList_FindByValue() {
    SparseList<int> list(15, 0);
    list.set(4, 99);
    list.set(8, 55);
    
    int index = list.findByValue(99);
    bool passed = (index == 4);
    reportTest("SparseList - Find By Value", passed);
}

void test_SparseList_FindFirstBy() {
    SparseList<int> list(20, 0);
    list.set(3, 5);
    list.set(7, 15);
    list.set(12, 25);
    
    int index = list.findFirstBy([](const int& val) { return val > 10; });
    bool passed = (index == 7);
    reportTest("SparseList - Find First By Predicate", passed);
}

void test_SparseList_SaveLoad() {
    SparseList<int> list1(10, 0);
    list1.set(2, 42);
    list1.set(5, 100);
    list1.saveToFile("test_list.txt");
    
    SparseList<int> list2;
    list2.loadFromFile("test_list.txt");
    
    bool passed = (list2.get(2) == 42) && (list2.get(5) == 100) && (list2.size() == 10);
    reportTest("SparseList - Save and Load", passed);
}

void test_SparseMatrix_SetGet() {
    MapSparseMatrix<int> matrix(5, 5, 0);
    matrix.set(1, 2, 42);
    matrix.set(3, 4, 99);
    
    bool passed = (matrix.get(1, 2) == 42) && (matrix.get(3, 4) == 99) && (matrix.get(0, 0) == 0);
    reportTest("SparseMatrix - Set and Get", passed);
}

void test_SparseMatrix_Transpose() {
    MapSparseMatrix<int> matrix(3, 4, 0);
    matrix.set(0, 2, 10);
    matrix.set(2, 3, 20);
    
    auto transposed = unique_ptr<SparseMatrix<int>>(matrix.transpose());
    
    bool passed = (transposed->getRows() == 4) && (transposed->getCols() == 3) &&
                  (transposed->get(2, 0) == 10) && (transposed->get(3, 2) == 20);
    reportTest("SparseMatrix - Transpose", passed);
}

void test_SparseMatrix_Addition() {
    MapSparseMatrix<int> matrix1(3, 3, 0);
    matrix1.set(0, 0, 5);
    matrix1.set(1, 1, 10);
    
    MapSparseMatrix<int> matrix2(3, 3, 0);
    matrix2.set(0, 0, 3);
    matrix2.set(1, 1, 7);
    
    auto result = unique_ptr<SparseMatrix<int>>(matrix1.add(matrix2));
    
    bool passed = (result->get(0, 0) == 8) && (result->get(1, 1) == 17);
    reportTest("SparseMatrix - Addition", passed);
}

void test_SparseMatrix_VectorMultiply() {
    MapSparseMatrix<int> matrix(2, 3, 0);
    matrix.set(0, 0, 1);
    matrix.set(0, 1, 2);
    matrix.set(0, 2, 3);
    matrix.set(1, 0, 4);
    matrix.set(1, 1, 5);
    matrix.set(1, 2, 6);
    
    vector<int> vec = {1, 2, 3};
    auto result = matrix.multiplyVector(vec);
    
    bool passed = (result[0] == 14) && (result[1] == 32);
    reportTest("SparseMatrix - Vector Multiplication", passed);
}

void test_MathExpression_Constant() {
    auto constant = make_shared<Constant>(42.0);
    
    bool passed = (constant->evaluate(0) == 42.0) && (constant->evaluate(100) == 42.0);
    reportTest("MathExpression - Constant", passed);
}

void test_MathExpression_Variable() {
    auto variable = make_shared<Variable>();
    
    bool passed = (variable->evaluate(5.0) == 5.0) && (variable->evaluate(10.0) == 10.0);
    reportTest("MathExpression - Variable", passed);
}

void test_MathExpression_Sum() {
    auto x = make_shared<Variable>();
    auto five = make_shared<Constant>(5);
    auto sum = make_shared<Sum>(x, five);
    
    bool passed = (sum->evaluate(3.0) == 8.0) && (sum->evaluate(10.0) == 15.0);
    reportTest("MathExpression - Sum", passed);
}

void test_MathExpression_Derivative() {
    auto x = make_shared<Variable>();
    auto x2 = make_shared<Power>(x, 2);
    
    auto derivative = x2->derivative();
    
    double result = derivative->evaluate(3.0);
    bool passed = (abs(result - 6.0) < 0.01);
    reportTest("MathExpression - Derivative of x^2", passed);
}

void test_MathFunction_Integration() {
    auto x = make_shared<Variable>();
    MathFunction func(x, "f");
    
    double integral = func.integrate(0, 2, 1000);
    bool passed = (abs(integral - 2.0) < 0.01);
    reportTest("MathFunction - Integration", passed);
}