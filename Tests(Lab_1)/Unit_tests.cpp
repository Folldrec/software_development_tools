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