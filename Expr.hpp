/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_ARITHEXPR_HPP
#define EXPRINTER_ARITHEXPR_HPP

#include "SymTab.hpp"

// Classes in this file define the internal representation of arithmetic
// expressions.

// An ExprNode serves as the base class for expressions.
// It forces the derived classes (subclasses) to implement two functions
// Print and Evaluate.
class ExprNode {
public:
    explicit ExprNode(Token token);
    Token token();
    virtual void print() = 0;
	virtual std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) = 0;
	virtual ~ExprNode() = default;
private:
    Token _token;
};

// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ExprNode {  // An expression tree node.
public:
    explicit InfixExprNode(Token tk);
	std::unique_ptr<ExprNode> &left();
	std::unique_ptr<ExprNode> &right();
    void print() override;
	std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) override;
private:
	std::unique_ptr<ExprNode> _left;
	std::unique_ptr<ExprNode> _right;
};

// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class WholeNumber: public ExprNode {
public:
    explicit WholeNumber(Token token);
    void print() override;
	std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) override;
};

// Float is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class Float: public ExprNode {
public:
	explicit Float(Token token);
	void print() override;
	std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) override;
};

// Variable is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class Variable: public ExprNode {
public:
    explicit Variable(Token token);
    void print() override;
	std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) override;
};

// String is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.
class String: public ExprNode {
public:
    explicit String(Token token);
    void print() override;
	std::shared_ptr<TypeDescriptor> evaluate(SymTab &symTab) override;
};

#endif //EXPRINTER_ARITHEXPR_HPP
