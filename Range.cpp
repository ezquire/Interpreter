#include "Range.hpp"
#include "Expr.hpp"
#include "Statements.hpp"
#include "SymTab.hpp"

#include<vector>

Range::Range(): _start{0}, _end{0}, _step{0} {}

Range::Range( int start, int end, int step ):
	_start{start}, _end{end}, _step{step} {}

Range::Range(std::string varName, std::vector<ExprNode *> rangeList,
			 SymTab &symTab) {
	if(rangeList.size() < 1 || rangeList.size() > 3) {
		std::cout << "Error: Incorrect number of arguments to Range()\n";
		exit(1);
	} else if(rangeList.size() == 1) { // only have an end value

		NumberDescriptor*start = new NumberDescriptor(TypeDescriptor::INTEGER);
		start->value.intValue = 0; // set start to 0 by default
		symTab.setValueFor( varName, start ); // set value of ID to start
		
		auto end = rangeList[0]->evaluate(symTab);
		NumberDescriptor *endVal = dynamic_cast<NumberDescriptor *>(end);

		if( endVal == nullptr ) {
			std::cout << "Range::Range error casting TypeDescriptor\n";
			exit(1);
		} else if( endVal->type() != TypeDescriptor::INTEGER ) {
			std::cout << "Error: range can only accept integer values\n";
			exit(1);
		} else {
			_start = 0;
			_end = endVal->value.intValue;
			_step = 1; 
		}
	} else if(rangeList.size() == 2) { // start and end only, step is 1
		auto start = rangeList[0]->evaluate(symTab);
		symTab.setValueFor( varName, start ); // set value of ID to start
		
		auto end = rangeList[1]->evaluate(symTab);
		NumberDescriptor *startVal = dynamic_cast<NumberDescriptor *>(start);
		NumberDescriptor *endVal = dynamic_cast<NumberDescriptor *>(end);
		if( startVal == nullptr || endVal == nullptr ) {
			std::cout << "Range::Range error casting TypeDescriptor\n";
			exit(1);
		} else if( startVal->type() != TypeDescriptor::INTEGER
				   || endVal->type() != TypeDescriptor::INTEGER ) {
			std::cout << "Error: range can only accept integer values\n";
			exit(1);
		} else {
			_start = startVal->value.intValue;
			_end = endVal->value.intValue;
			_step = 1; 
		}
	} else { // start, end, and step
		auto start = rangeList[0]->evaluate(symTab);
		symTab.setValueFor( varName, start ); // set value of ID to start
		auto end = rangeList[1]->evaluate(symTab);
		auto step = rangeList[2]->evaluate(symTab);
		NumberDescriptor *startVal = dynamic_cast<NumberDescriptor *>(start);
		NumberDescriptor *endVal = dynamic_cast<NumberDescriptor *>(end);
		NumberDescriptor *stepVal = dynamic_cast<NumberDescriptor *>(step);
		if( startVal == nullptr || endVal == nullptr || stepVal == nullptr) {
			std::cout << "Range::Range error casting TypeDescriptor\n";
			exit(1);
		} else if( startVal->type() != TypeDescriptor::INTEGER
				   || endVal->type() != TypeDescriptor::INTEGER
				   || stepVal->type() != TypeDescriptor::INTEGER ) {
			std::cout << "Error: range can only accept integer values\n";
			exit(1);
		} else {
			_start = startVal->value.intValue;
			_end = endVal->value.intValue;
			_step = stepVal->value.intValue; 
		}
	}
}

int Range::getNext() {
	return _start += _step;	
}

bool Range::atEnd() {
	if(_start > _end && _step < 0)
		return _start <= _end;
	else
		return _start >= _end;
}

int &Range::start() {
	return _start;
}

int &Range::end() {
	return _end;
}

int &Range::step() {
	return _step;
}
