//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}


// Statements
Statements::Statements() {}

void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}


// AssignmentStatement
AssignmentStatement::AssignmentStatement(): _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}


// PrintStatement
PrintStatement::PrintStatement() : _rhsExpression{nullptr} {}

PrintStatement::PrintStatement(ExprNode *rhsExpr):
        _rhsExpression{rhsExpr} {}

void PrintStatement::evaluate(SymTab &symTab) {
	std::cout << rhsExpression()->evaluate(symTab) << std::endl;
}

ExprNode *&PrintStatement::rhsExpression() {
    return _rhsExpression;
}

void PrintStatement::print() {
    _rhsExpression->print();
    std::cout << std::endl;
}


// ForStatement
ForStatement::ForStatement() : _firstAssign{nullptr}, _midExpr{nullptr}, _secondAssign{nullptr}, _statements{nullptr}  {}

ForStatement::ForStatement(AssignmentStatement *firstAssign, ExprNode *midExpr, AssignmentStatement *secondAssign, Statements* stmts):
	_firstAssign{firstAssign}, _midExpr{midExpr}, _secondAssign{secondAssign}, _statements{stmts} {}

void ForStatement::evaluate(SymTab &symTab) {
	firstAssign()->evaluate(symTab); 
    while( midExpr()->evaluate(symTab) ) {
		statements()->evaluate(symTab);
		secondAssign()->evaluate(symTab);
	}
}

AssignmentStatement *&ForStatement::firstAssign() {
    return _firstAssign;
}

AssignmentStatement *&ForStatement::secondAssign() {
    return _secondAssign;
}

ExprNode *&ForStatement::midExpr() {
    return _midExpr;
}

Statements *&ForStatement::statements() {
	return _statements;
}

void ForStatement::print() {
	_firstAssign->print();
    std::cout << std::endl;
    _midExpr->print();
    std::cout << std::endl;
	_secondAssign->print();
    std::cout << std::endl;
	_statements->print();
    std::cout << std::endl;
}
