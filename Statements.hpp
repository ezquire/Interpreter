/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "Range.hpp"
#include "Expr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements
// defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.
class Statement {
public:
    Statement();
    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
    virtual ~Statement() = default;
};

// Statements is a collection of Statement. For example, all statements in a
// function can be represented by an instance of Statements.
class Statements {
public:
    Statements();
    void addStatement(std::unique_ptr<Statement> statement);
    void evaluate(SymTab &symTab);
    void print();
private:
    std::vector<std::unique_ptr<Statement>> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned
// an rValue. The rValue is an expression.
class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, std::unique_ptr<ExprNode> rhsExpr);
    std::string &lhsVariable();
	std::unique_ptr<ExprNode> &rhsExpression();
    void evaluate (SymTab &symTab) override;
    void print() override;
private:
    std::string _lhsVariable;
	std::unique_ptr<ExprNode> _rhsExpression;
};

// PrintStatement
class PrintStatement : public Statement {
public:
	PrintStatement();
    explicit PrintStatement(std::vector<std::shared_ptr<ExprNode>> rhsList);
	std::vector<std::shared_ptr<ExprNode>> &rhsList();
    void evaluate(SymTab &symTab) override;
    void print() override;
private:
	std::vector<std::shared_ptr<ExprNode>> _rhsList;
};

// CallStatement
class CallStatement : public Statement {
public:
	CallStatement();
	explicit CallStatement(std::string id,
						   std::vector<std::shared_ptr<ExprNode>> callList);
	std::vector<std::shared_ptr<ExprNode>> &callList();
	void evaluate(SymTab &symTab) override;
	void print() override;
private:
	std::string _id;
	std::vector<std::shared_ptr<ExprNode>> _callList;
};

// ForStatement
class ForStatement : public Statement {
public:
	ForStatement();
	ForStatement(std::string id, std::vector<std::shared_ptr<ExprNode>> range,
				 std::unique_ptr<Statements> stmnts);
	std::unique_ptr<Statements> &statements();
	std::vector<std::shared_ptr<ExprNode>> &getRange();
	std::string &getId();
    void evaluate(SymTab &symTab) override;
    void print() override;
private:
	std::string _id;
	std::vector<std::shared_ptr<ExprNode>> _range;
	std::unique_ptr<Statements> _statements;
};

// IfStatement
class IfStatement : public Statement {
public:
	IfStatement();
 	IfStatement(std::unique_ptr<ExprNode> firstTest,
				std::unique_ptr<Statements> firstSuite,
				std::vector<std::unique_ptr<ExprNode>> elifTests,
				std::vector<std::unique_ptr<Statements>> elifSuites,
				std::unique_ptr<Statements> elseSuite);
	std::unique_ptr<ExprNode> &firstTest();
	std::unique_ptr<Statements> &firstSuite();
	std::vector<std::unique_ptr<ExprNode>> &elifTests();
	std::vector<std::unique_ptr<Statements>> &elifSuites();
	std::unique_ptr<Statements> &elseSuite(); 
    void evaluate(SymTab &symTab) override;
    void print() override;
private:
	std::unique_ptr<ExprNode> _firstTest;
	std::unique_ptr<Statements> _firstSuite;
	std::vector<std::unique_ptr<ExprNode>> _elifTests;
	std::vector<std::unique_ptr<Statements>> _elifSuites;
	std::unique_ptr<Statements> _elseSuite;
};

class Function : public Statement {
public:
	Function();
	Function(std::string id, std::vector<std::string> params,
			 std::unique_ptr<Statements> suite);
	void evaluate(SymTab &symTab) override;
	void print() override;
	std::string &id();
	std::vector<std::string> &params();
	std::unique_ptr<Statements> &suite();
private:
	std::string _id;
	std::vector<std::string> _parameters;
	std::unique_ptr<Statements> _suite;
};

#endif //EXPRINTER_STATEMENTS_HPP
