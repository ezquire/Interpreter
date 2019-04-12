/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_ARITHEXPR_HPP
#define EXPRINTER_ARITHEXPR_HPP

#include "TypeDescriptor.hpp"
#include "Token.hpp"
#include "SymTab.hpp"

// Classes in this file define the internal representation of arithmetic expressions.


// An ExprNode serves as the base class for expressions.
// It forces the derived classes (subclasses) to implement two functions
// Print and Evaluate.
class ExprNode {
public:
    ExprNode(Token token);
    Token token();
    virtual void print() = 0;
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) = 0;
	virtual ~ExprNode() {}
private:
    Token _token;
};


// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ExprNode {  // An expression tree node.

public:
    InfixExprNode(Token tk);
	std::unique_ptr<ExprNode> &left();
	std::unique_ptr<ExprNode> &right();
    virtual void print();
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab);
private:
	std::unique_ptr<ExprNode> _left;
	std::unique_ptr<ExprNode> _right;
};


// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class WholeNumber: public ExprNode {
public:
    WholeNumber(Token token);
    virtual void print();
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab);
};


// Float is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class Float: public ExprNode {
public:
	Float(Token token);
	virtual void print();
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab);
};


// Variable is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class Variable: public ExprNode {
public:
    Variable(Token token);
    virtual void print();
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab);
};


// String is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class String: public ExprNode {
public:
    String(Token token);
    virtual void print();
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab);
};

#endif //EXPRINTER_ARITHEXPR_HPP
