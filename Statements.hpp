//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ArithExpr.hpp"
#include "SymTab.hpp"
//#include "Range.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.
class range {
public:
	range();
        bool Range(int a, int b, int c);
        int getStart() {return start;}
        void setStart(int n) { start = n;}
	int getEnd() {return end;}
	void setEnd(int n) {end = n;}
	int getStep() {return step;}
	void setStep(int n) {step = n;} 


private:
	int start;
	int end;
	int step;
};

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

// PrintStatement represents the notion of of having a keyword followed by a name
// the name has been assigned a value which is to be printed on the screen.
class PrintStatement : public Statement {
public:
    PrintStatement();
    PrintStatement(std::vector<ExprNode *> pvector);

    virtual void evaluate(SymTab & symTab);
    virtual void print();

private:
    std::vector<ExprNode *> _StrVector;

};
// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);
    //
    AssignmentStatement(std::string lhsVar, std::string rhsVar);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();
    std::string &rhsVariable();

    void printFor();
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
    std::string _rhsVariable;
};

// ForStatement
class ForStatement : public Statement {
public:
     ForStatement();
     ForStatement(std::vector<ExprNode *> fVector, Statements * stmts);
 
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:

    Statements * _statements;
    std::vector<ExprNode *> _forVector;


};

// if statement
class IfStatement : public Statement {
public:
    
    IfStatement();
    IfStatement(ExprNode * Test, Statements * If, std::vector<ExprNode *> Test2vect, std::vector<Statements * >Elif, Statements * Else);
    virtual void evaluate(SymTab & symtab);
    virtual void print();
    
private:
    ExprNode * _test;
    std::vector<ExprNode *> _testVect;
    Statements * _if;
    std::vector<Statements *> _elif;
    Statements * _else;

    
};
#endif //EXPRINTER_STATEMENTS_HPP
