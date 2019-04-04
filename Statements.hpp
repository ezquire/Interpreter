//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "Range.hpp"
#include "Expr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.
class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

// PrintStatement
class PrintStatement : public Statement {
public:
	PrintStatement();
    PrintStatement(std::vector<ExprNode *>rhsList);

	std::vector<ExprNode *>&rhsList();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
	std::vector<ExprNode *>_rhsList;
};

// ForStatement
class ForStatement : public Statement {
public:
	ForStatement();
	ForStatement(std::string id, std::vector<ExprNode *> range,
				 Statements *stmnts);

	Statements *&statements();
	std::vector<ExprNode *> &getRange();
	std::string &getId();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
	std::vector<ExprNode *> _range;
	Statements *_statements;
	std::string _id;
};

// IfStatement
class IfStatement : public Statement {
public:
	IfStatement();
	IfStatement(ExprNode *firstTest, Statements *firstSuite);
 	IfStatement(ExprNode *firstTest, Statements *firstSuite,
				std::vector<ExprNode *> elifTests,
				std::vector<Statements *> elifSuites,
				Statements *elseSuite);

	ExprNode *&firstTest();
	Statements *&firstSuite();
	std::vector<ExprNode *> &elifTests();
	std::vector<Statements *> &elifSuites();
	Statements *&elseSuite(); 
	
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
	ExprNode *_firstTest;
	Statements *_firstSuite;
	std::vector<ExprNode *>_elifTests;
	std::vector<Statements *>_elifSuites;
	Statements *_elseSuite;
};


#endif //EXPRINTER_STATEMENTS_HPP
