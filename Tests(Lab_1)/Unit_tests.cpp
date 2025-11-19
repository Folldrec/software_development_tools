#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include <fstream>
#include "../MathExpression.h"
#include "../MathFunction.h"
#include "../Sequence.h"
#include "../SparseList.h"
#include "../SparseMatrix.h"
#include "../ComputerAlgebraInterface.h"

TEST(MathExpressionTest, ConstantEvaluation) {
    Constant c(5.0);
    EXPECT_DOUBLE_EQ(c.evaluate(10.0), 5.0);
    EXPECT_DOUBLE_EQ(c.evaluate(0.0), 5.0);
    EXPECT_EQ(c.toString(), "5");
}

TEST(MathExpressionTest, VariableEvaluation) {
    Variable x;
    EXPECT_DOUBLE_EQ(x.evaluate(3.0), 3.0);
    EXPECT_DOUBLE_EQ(x.evaluate(-2.5), -2.5);
    EXPECT_EQ(x.toString(), "x");
}

TEST(MathExpressionTest, BasicDerivatives) {
    Constant c(10.0);
    auto cDeriv = c.derivative();
    EXPECT_DOUBLE_EQ(cDeriv->evaluate(5.0), 0.0);
    
    Variable x;
    auto xDeriv = x.derivative();
    EXPECT_DOUBLE_EQ(xDeriv->evaluate(5.0), 1.0);
}

TEST(MathExpressionTest, SumOperation) {
    auto x = std::make_shared<Variable>();
    auto c = std::make_shared<Constant>(3.0);
    Sum sum(x, c);
    
    EXPECT_DOUBLE_EQ(sum.evaluate(2.0), 5.0);
    EXPECT_DOUBLE_EQ(sum.evaluate(0.0), 3.0);
    
    auto sumDeriv = sum.derivative();
    EXPECT_DOUBLE_EQ(sumDeriv->evaluate(10.0), 1.0);
}

TEST(MathExpressionTest, ProductOperation) {
    auto x = std::make_shared<Variable>();
    auto c = std::make_shared<Constant>(2.0);
    Product prod(c, x);
    
    EXPECT_DOUBLE_EQ(prod.evaluate(3.0), 6.0);
    EXPECT_DOUBLE_EQ(prod.evaluate(0.0), 0.0);
    
    auto prodDeriv = prod.derivative();
    EXPECT_DOUBLE_EQ(prodDeriv->evaluate(5.0), 2.0);
}

TEST(MathFunctionTest, PolynomialEvaluation) {
    auto x = std::make_shared<Variable>();
    auto x2 = std::make_shared<Power>(x, 2);
    auto two = std::make_shared<Constant>(2);
    auto twoX = std::make_shared<Product>(two, x);
    auto one = std::make_shared<Constant>(1);
    auto sum1 = std::make_shared<Sum>(x2, twoX);
    auto polynomial = std::make_shared<Sum>(sum1, one);
    
    MathFunction func(polynomial, "f");
    
    EXPECT_DOUBLE_EQ(func.evaluate(0.0), 1.0); 
    EXPECT_DOUBLE_EQ(func.evaluate(1.0), 4.0);  
    EXPECT_DOUBLE_EQ(func.evaluate(2.0), 9.0); 
}

TEST(MathFunctionTest, PolynomialDerivative) {
    auto x = std::make_shared<Variable>();
    auto x2 = std::make_shared<Power>(x, 2);
    MathFunction func(x2, "f");
    
    auto deriv = func.derivative();
    EXPECT_NEAR(deriv.evaluate(3.0), 6.0, 1e-6);  
    EXPECT_NEAR(deriv.evaluate(5.0), 10.0, 1e-6); 
}

TEST(MathFunctionTest, Integration) {
    auto x = std::make_shared<Variable>();
    MathFunction func(x, "f");
    
    double integral = func.integrate(0.0, 1.0, 1000);
    EXPECT_NEAR(integral, 0.5, 0.01);
}

TEST(SequenceTest, ArithmeticSequence) {
    ArithmeticSequence seq(1.0, 2.0); 
    
    EXPECT_DOUBLE_EQ(seq.getTerm(1), 1.0);  
    EXPECT_DOUBLE_EQ(seq.getTerm(2), 3.0); 
    EXPECT_DOUBLE_EQ(seq.getTerm(5), 9.0); 
    
    EXPECT_DOUBLE_EQ(seq.partialSum(1, 5), 25.0);
}

TEST(SequenceTest, GeometricSequence) {
    GeometricSequence seq(2.0, 0.5); 
    
    EXPECT_DOUBLE_EQ(seq.getTerm(1), 2.0);  
    EXPECT_DOUBLE_EQ(seq.getTerm(2), 1.0);  
    EXPECT_DOUBLE_EQ(seq.getTerm(3), 0.5);  
    
    EXPECT_TRUE(seq.checkConvergence());
}

TEST(SparseListTest, BasicOperations) {
    SparseList<int> list(10, 0);
    
    list.set(2, 5);
    list.set(7, 10);
    
    EXPECT_EQ(list.get(2), 5);
    EXPECT_EQ(list.get(7), 10);
    EXPECT_EQ(list.get(0), 0);
    EXPECT_EQ(list.size(), 10);
    EXPECT_EQ(list.nonZeroCount(), 2);
}

TEST(SparseListTest, FindOperations) {
    SparseList<int> list(20, 0);
    
    list.set(5, 15);
    list.set(10, 25);
    
    EXPECT_EQ(list.findByValue(15), 5);
    EXPECT_EQ(list.findByValue(25), 10);
    EXPECT_EQ(list.findByValue(100), -1);  
    
    int firstLarge = list.findFirstBy([](const int& val) { return val > 20; });
    EXPECT_EQ(firstLarge, 10);
}

TEST(SparseMatrixTest, MatrixAddition) {
    MapSparseMatrix<int> m1(3, 3, 0);
    MapSparseMatrix<int> m2(3, 3, 0);
    
    m1.set(0, 0, 1);
    m1.set(1, 1, 2);
    
    m2.set(0, 0, 3);
    m2.set(2, 2, 4);
    
    auto result = std::unique_ptr<SparseMatrix<int>>(m1.add(m2));
    
    EXPECT_EQ(result->get(0, 0), 4); 
    EXPECT_EQ(result->get(1, 1), 2); 
    EXPECT_EQ(result->get(2, 2), 4);  
}

TEST(SparseMatrixTest, MatrixVectorMultiply) {
    MapSparseMatrix<int> matrix(2, 3, 0);
    
    matrix.set(0, 0, 1);
    matrix.set(0, 1, 2);
    matrix.set(1, 1, 3);
    matrix.set(1, 2, 4);
    
    std::vector<int> vec = {1, 2, 3};
    auto result = matrix.multiplyVector(vec);
    
    EXPECT_EQ(result[0], 5);  
    EXPECT_EQ(result[1], 18); 
}

