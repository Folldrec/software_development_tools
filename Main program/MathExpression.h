#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H

#include <string>
#include <memory>
#include <cmath>
#include <sstream>
#include <map>
#include <vector>

class Cos;
class Sin;

class MathExpression {
public:
    virtual ~MathExpression() = default;
    
    virtual double evaluate(double x) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<MathExpression> derivative() const = 0;
    virtual std::shared_ptr<MathExpression> clone() const = 0;
};

class Constant : public MathExpression {
private:
    double value;
    
public:
    Constant(double v) : value(v) {}
    
    double evaluate(double x) const override {
        return value;
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        return std::make_shared<Constant>(0);
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Constant>(value);
    }
};

class Variable : public MathExpression {
public:
    double evaluate(double x) const override {
        return x;
    }
    
    std::string toString() const override {
        return "x";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        return std::make_shared<Constant>(1);
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Variable>();
    }
};

class Sum : public MathExpression {
private:
    std::shared_ptr<MathExpression> left;
    std::shared_ptr<MathExpression> right;
    
public:
    Sum(std::shared_ptr<MathExpression> l, std::shared_ptr<MathExpression> r)
        : left(l), right(r) {}
    
    double evaluate(double x) const override {
        return left->evaluate(x) + right->evaluate(x);
    }
    
    std::string toString() const override {
        return "(" + left->toString() + " + " + right->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        return std::make_shared<Sum>(left->derivative(), right->derivative());
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Sum>(left->clone(), right->clone());
    }
};

class Product : public MathExpression {
private:
    std::shared_ptr<MathExpression> left;
    std::shared_ptr<MathExpression> right;
    
public:
    Product(std::shared_ptr<MathExpression> l, std::shared_ptr<MathExpression> r)
        : left(l), right(r) {}
    
    double evaluate(double x) const override {
        return left->evaluate(x) * right->evaluate(x);
    }
    
    std::string toString() const override {
        return "(" + left->toString() + " * " + right->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        std::shared_ptr<MathExpression> term1 = std::make_shared<Product>(left->derivative(), right->clone());
        std::shared_ptr<MathExpression> term2 = std::make_shared<Product>(left->clone(), right->derivative());
        return std::make_shared<Sum>(term1, term2);
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Product>(left->clone(), right->clone());
    }
};

class Power : public MathExpression {
private:
    std::shared_ptr<MathExpression> base;
    double exponent;
    
public:
    Power(std::shared_ptr<MathExpression> b, double exp)
        : base(b), exponent(exp) {}
    
    double evaluate(double x) const override {
        return std::pow(base->evaluate(x), exponent);
    }
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << "(" << base->toString() << ")^" << exponent;
        return oss.str();
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        std::shared_ptr<MathExpression> coef = std::make_shared<Constant>(exponent);
        std::shared_ptr<MathExpression> pow = std::make_shared<Power>(base->clone(), exponent - 1);
        std::shared_ptr<MathExpression> prod1 = std::make_shared<Product>(coef, pow);
        return std::make_shared<Product>(prod1, base->derivative());
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Power>(base->clone(), exponent);
    }
};

class Cos : public MathExpression {
private:
    std::shared_ptr<MathExpression> arg;
    
public:
    Cos(std::shared_ptr<MathExpression> a) : arg(a) {}
    
    double evaluate(double x) const override {
        return std::cos(arg->evaluate(x));
    }
    
    std::string toString() const override {
        return "cos(" + arg->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override;
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Cos>(arg->clone());
    }
};

class Sin : public MathExpression {
private:
    std::shared_ptr<MathExpression> arg;
    
public:
    Sin(std::shared_ptr<MathExpression> a) : arg(a) {}
    
    double evaluate(double x) const override {
        return std::sin(arg->evaluate(x));
    }
    
    std::string toString() const override {
        return "sin(" + arg->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        std::shared_ptr<MathExpression> cosExpr = std::make_shared<Cos>(arg->clone());
        return std::make_shared<Product>(cosExpr, arg->derivative());
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Sin>(arg->clone());
    }
};

inline std::shared_ptr<MathExpression> Cos::derivative() const {
    std::shared_ptr<MathExpression> minusOne = std::make_shared<Constant>(-1);
    std::shared_ptr<MathExpression> sinExpr = std::make_shared<Sin>(arg->clone());
    std::shared_ptr<MathExpression> prod = std::make_shared<Product>(minusOne, sinExpr);
    return std::make_shared<Product>(prod, arg->derivative());
}

class Exp : public MathExpression {
private:
    std::shared_ptr<MathExpression> arg;
    
public:
    Exp(std::shared_ptr<MathExpression> a) : arg(a) {}
    
    double evaluate(double x) const override {
        return std::exp(arg->evaluate(x));
    }
    
    std::string toString() const override {
        return "exp(" + arg->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        std::shared_ptr<MathExpression> expExpr = std::make_shared<Exp>(arg->clone());
        return std::make_shared<Product>(expExpr, arg->derivative());
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Exp>(arg->clone());
    }
};

class Ln : public MathExpression {
private:
    std::shared_ptr<MathExpression> arg;
    
public:
    Ln(std::shared_ptr<MathExpression> a) : arg(a) {}
    
    double evaluate(double x) const override {
        return std::log(arg->evaluate(x));
    }
    
    std::string toString() const override {
        return "ln(" + arg->toString() + ")";
    }
    
    std::shared_ptr<MathExpression> derivative() const override {
        std::shared_ptr<MathExpression> invArg = std::make_shared<Power>(arg->clone(), -1);
        return std::make_shared<Product>(arg->derivative(), invArg);
    }
    
    std::shared_ptr<MathExpression> clone() const override {
        return std::make_shared<Ln>(arg->clone());
    }
};

#endif