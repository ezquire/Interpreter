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
    for (auto &s: _statements) {
        s->evaluate(symTab);
	}
}

// AssignmentStatement
AssignmentStatement::AssignmentStatement(): _lhsVariable{""}, _rhsExpression{nullptr}, _array{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar,
										 std::unique_ptr<ExprNode> rhsExpr):
	_lhsVariable{std::move(lhsVar)}, _rhsExpression{std::move(rhsExpr)} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, std::vector<std::shared_ptr<ExprNode>> array) {
    _lhsVariable = lhsVar;
    _array = array;
}
/*
AssignmentStatement::AssignmentStatement(std::string lhsVar):

  _lhsVariable{std::move(lhsVar)} {}
*/
void AssignmentStatement::evaluate(SymTab &symTab) {
    
    if(rhsExpression() != nullptr){
        symTab.setValueFor(lhsVariable(),
					   rhsExpression()->evaluate(symTab));
    }
    //case with empty array
    else if(arrSize() == 0){
        std::shared_ptr<TypeDescriptor> desc = std::make_shared<TypeDescriptor>(TypeDescriptor::NULLARRAY);
        
        symTab.setValueFor(lhsVariable(), desc);
    }
    //case with non-empty array
    else if(arrSize() > 0){
        //case for string arrays
        if(_array[0]->evaluate(symTab)->type() == TypeDescriptor::STRING){
            //create string array type descriptor
            std::shared_ptr<StringArray> desc =
               std::make_shared<StringArray>(StringArray::STRINGARRAY);
            //create vector to hold string array items
            std::vector<std::string> _stringArray;
            //push items into string array
            for(int i = 0; i<= _array.size()-1; i++){

                auto sIDesc = dynamic_cast<StringDescriptor*>(_array[i]->evaluate(symTab).get());
                _stringArray.push_back(sIDesc->value);
            }
            //set stringArray in String array descriptor to hold vector of strings
            desc->stringArray = _stringArray;
            symTab.setValueFor(lhsVariable(), desc);
        }
        else if(_array[0]->evaluate(symTab)->type() == TypeDescriptor::INTEGER){
            //create string array type descriptor
            
            std::shared_ptr<NumberArray> Ndesc =
              std::make_shared<NumberArray>(NumberArray::NUMBERARRAY);
            //create vector to hold string array items
            std::vector<int> _numberArray;
            //push items into string array
            
            for(int i = 0; i<= _array.size()-1; i++){
                
                auto sIDesc = dynamic_cast<NumberDescriptor*>(_array[i]->evaluate(symTab).get());
                _numberArray.push_back(sIDesc->value.intValue);
            }
            //set stringArray in String array descriptor to hold vector of strings
            Ndesc->numberArray = _numberArray;
            symTab.setValueFor(lhsVariable(), Ndesc);
            
        }
    
    }
}

int AssignmentStatement::arrSize() {
    return _array.size();
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
PrintStatement::PrintStatement() : _rhsList{} {}

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
	//print vectors
	//print vectors
	//_elseSuite->print();
	//std::cout << std::endl;
}

//ARRAYOPS
ArrayOps::ArrayOps() : _s{""}, _s1{""} {}

ArrayOps::ArrayOps(std::string s, std::string s1){
    _s = s;
    _s1 = s1;
}


void ArrayOps::evaluate(SymTab & symTab) {
    
    std::cout<<"hi";
}

void ArrayOps::print(){}
