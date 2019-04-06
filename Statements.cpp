//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Range.hpp"
#include "Statements.hpp"
#include "TypeDescriptor.hpp"

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
    for (auto s: _statements) {
        s->evaluate(symTab);
	}
}


// AssignmentStatement
AssignmentStatement::AssignmentStatement(): _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    symTab.setValueFor(lhsVariable(), rhsExpression()->evaluate(symTab) );
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
PrintStatement::PrintStatement() : _rhsList{nullptr} {}

PrintStatement::PrintStatement(std::vector<ExprNode *> exprList):
        _rhsList{exprList} {}

void PrintStatement::evaluate(SymTab &symTab) {
	for (auto l: _rhsList ) {
        printValue( l->evaluate(symTab) );
		std::cout << ' ';
	}
	std::cout << std::endl;
}

std::vector<ExprNode *>&PrintStatement::rhsList() {
    return _rhsList;
}

void PrintStatement::print() {
	for (auto list: _rhsList) {
		list->print();
		std::cout << std::endl;
	}
}


// ForStatement
ForStatement::ForStatement() : _statements{nullptr}  {}

ForStatement::ForStatement(std::string id, std::vector<ExprNode *>range, Statements* stmnts):
	_id{id}, _range{range}, _statements{stmnts} {}

void ForStatement::evaluate(SymTab &symTab) {
	
	Range *range = new Range(getId(), getRange(), symTab);
	
	while( !range->atEnd() ) {
		statements()->evaluate(symTab);
		symTab.increment( getId(), range->step() );
		range->getNext();
	}    
}

Statements *&ForStatement::statements() {
	return _statements;
}

std::vector<ExprNode *> &ForStatement::getRange() {
	return _range;
}

std::string &ForStatement::getId() {
	return _id;
}

void ForStatement::print() {
 	_statements->print();
    std::cout << std::endl;
}


// IfStatement
IfStatement::IfStatement() : _firstTest{nullptr}, _firstSuite{nullptr},
							 _elseSuite{nullptr}  {}

IfStatement::IfStatement(ExprNode *firstTest, Statements *firstSuite):
	_firstTest{firstTest}, _firstSuite{firstSuite} {}

IfStatement::IfStatement(ExprNode *firstTest, Statements *firstSuite,
						 std::vector<ExprNode *> elifTests,
						 std::vector<Statements *> elifSuites,
						 Statements *elseSuite):
	_firstTest{firstTest}, _firstSuite{firstSuite}, _elifTests{elifTests},
	_elifSuites{elifSuites}, _elseSuite{elseSuite} {}

void IfStatement::evaluate(SymTab &symTab) {	
	if( evaluateBool(firstTest()->evaluate(symTab) ) )
		firstSuite()->evaluate(symTab);
	if(_elifTests.size() != _elifSuites.size() ) {
		std::cout << "IfStatement::evaluate mismatched elif and arguments\n";
		exit(1);
	} else if ( _elifTests.size() != 0 ) {
		int i = 0;
		for( auto t: _elifTests ) {
			if( evaluateBool( t->evaluate(symTab) ) )
				_elifSuites[i]->evaluate(symTab);
			++i;
		}
	} else if( _elseSuite != nullptr )
		_elseSuite->evaluate(symTab);
}


ExprNode *&IfStatement::firstTest() {
	return _firstTest;
}

Statements *&IfStatement::firstSuite() {
	return _firstSuite;
}

std::vector<ExprNode *> &IfStatement::elifTests() {
	return _elifTests;
}

std::vector<Statements *> &IfStatement::elifSuites() {
	return _elifSuites;
}

Statements *&IfStatement::elseSuite() {
	return _elseSuite;
}


void IfStatement::print() {
	_firstTest->print();
	std::cout << std::endl;
	_firstSuite->print();
    std::cout << std::endl;
	//print vectors
	//print vectors
	//_elseSuite->print();
	//std::cout << std::endl;
}
