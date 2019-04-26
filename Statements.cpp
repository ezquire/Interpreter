/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Range.hpp"
#include "Statements.hpp"
#include "TypeDescriptor.hpp"

// Statement
Statement::Statement() = default;

// Statements
Statements::Statements() = default;

void Statements::addStatement(std::unique_ptr<Statement> statement)
{ _statements.push_back(std::move(statement)); }

void Statements::print() {
    for (auto &s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
	for (auto &s: _statements)
		s->evaluate(symTab);
}

// AssignmentStatement
AssignmentStatement::AssignmentStatement(): _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar,
										 std::unique_ptr<ExprNode> rhsExpr):
	_lhsVariable{std::move(lhsVar)}, _rhsExpression{std::move(rhsExpr)} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    symTab.setValueFor(lhsVariable(),
					   rhsExpression()->evaluate(symTab));
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

std::unique_ptr<ExprNode>&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
	std::cout << std::endl;
}

// PrintStatement
PrintStatement::PrintStatement(): _rhsList{} {}

PrintStatement::PrintStatement(std::vector<std::shared_ptr<ExprNode>>exprList):
	_rhsList{std::move(exprList)} {}

void PrintStatement::evaluate(SymTab &symTab) {
	for (auto &l: _rhsList ) {
        printValue( l->evaluate(symTab).get() );
		std::cout << ' ';
	}
	std::cout << std::endl;
}

std::vector<std::shared_ptr<ExprNode>>&PrintStatement::rhsList() {
    return _rhsList;
}

void PrintStatement::print() {
	for (auto &l: _rhsList) {
		l->print();
		std::cout << std::endl;
	}
}

// CallStatement
CallStatement::CallStatement(): _callList{} {}

CallStatement::CallStatement(std::string id,
							 std::vector<std::shared_ptr<ExprNode>> callList):
	_id{id}, _callList{callList} {}

void CallStatement::evaluate(SymTab &symTab) {
	std::cout << "Need to implement CallStatement::evaluate" << std::endl;
	exit(1);
}

std::vector<std::shared_ptr<ExprNode>> &CallStatement::callList() {
	return _callList;
}

void CallStatement::print() {
	for(auto &l: _callList) {
		l->print();
		std::cout << std::endl;
	}
}

// ForStatement
ForStatement::ForStatement() : _statements{nullptr}  {}

ForStatement::ForStatement(std::string id,
						   std::vector<std::shared_ptr<ExprNode>> range,
						   std::unique_ptr<Statements> stmnts):
	_id{std::move(id)}, _range{std::move(range)}, _statements{std::move(stmnts)} {}

void ForStatement::evaluate(SymTab &symTab) {
	std::unique_ptr<Range> range =
		std::make_unique<Range>(getId(), getRange(), symTab);
	while( !range->atEnd() ) {
		statements()->evaluate(symTab);
		symTab.increment( getId(), range->step() );
		range->getNext();
	}    
}

std::unique_ptr<Statements>&ForStatement::statements() {
	return _statements;
}

std::vector<std::shared_ptr<ExprNode>> &ForStatement::getRange() {
	return _range;
}

std::string &ForStatement::getId() {
	return _id;
}

void ForStatement::print() {
	std::cout << _id << std::endl;
	for(auto &n : _range)
		n->print();
	std::cout << std::endl;
 	_statements->print();
    std::cout << std::endl;
}

// IfStatement
IfStatement::IfStatement() : _firstTest{nullptr}, _firstSuite{nullptr},
							 _elseSuite{nullptr}  {}

IfStatement::IfStatement(std::unique_ptr<ExprNode>firstTest,
						 std::unique_ptr<Statements>firstSuite,
						 std::vector<std::unique_ptr<ExprNode>> elifTests,
						 std::vector<std::unique_ptr<Statements>> elifSuites,
						 std::unique_ptr<Statements>elseSuite):
	_firstTest{std::move(firstTest)},
	_firstSuite{std::move(firstSuite)},
	_elifTests{std::move(elifTests)},
	_elifSuites{std::move(elifSuites)},
	_elseSuite{std::move(elseSuite)} {}

void IfStatement::evaluate(SymTab &symTab) {	
	if(evaluateBool(firstTest()->evaluate(symTab).get())) {
		firstSuite()->evaluate(symTab);
		return;
	} else if(_elifTests.size() != _elifSuites.size() ) {
		std::cout << "IfStatement::evaluate mismatched elif and arguments\n";
		exit(1);
	} else if ( !_elifTests.empty() ) {
		int i = 0;
		for( auto &t: _elifTests ) {
			if( evaluateBool( t->evaluate(symTab).get() ) ) {
				_elifSuites[i]->evaluate(symTab);
				return;
			}
			++i;
		}
	} else if( _elseSuite != nullptr )
		_elseSuite->evaluate(symTab);
}

std::unique_ptr<ExprNode> &IfStatement::firstTest() {
	return _firstTest;
}

std::unique_ptr<Statements> &IfStatement::firstSuite() {
	return _firstSuite;
}

std::vector<std::unique_ptr<ExprNode>> &IfStatement::elifTests() {
	return _elifTests;
}

std::vector<std::unique_ptr<Statements>> &IfStatement::elifSuites() {
	return _elifSuites;
}

std::unique_ptr<Statements> &IfStatement::elseSuite() {
	return _elseSuite;
}

void IfStatement::print() {
	_firstTest->print();
	std::cout << std::endl;
	_firstSuite->print();
    std::cout << std::endl;
	for(auto &t: _elifTests)
		t->print();
	std::cout << std::endl;
	for(auto &s: _elifSuites)
		s->print();
	std::cout << std::endl;
	_elseSuite->print();
	std::cout << std::endl;
}

// Function
Function::Function(): _id{""}, _suite{nullptr} {}

Function::Function(std::string id, std::vector<std::string> params,
				   std::unique_ptr<Statements> suite):
	_id{id}, _parameters{params}, _suite{std::move(suite)} {}

void Function::evaluate(SymTab &symTab) {
	std::cout << "Function::evaluate not implemented yet" << std::endl;
	exit(1);
}

std::string &Function::id() {
	return _id;
}

std::vector<std::string> &Function::params() {
	return _parameters;
}

std::unique_ptr<Statements> &Function::suite() {
	return _suite;
}

void Function::print() {
	std::cout << _id << std::endl;
	for(auto &p: _parameters)
		std::cout << p << std::endl;
	_suite->print();
    std::cout << std::endl;
}
