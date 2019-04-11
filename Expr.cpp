// Created by Tyler Gearing 4/4/2019

#include <iostream>
#include <memory>
#include <cmath>

#include "Expr.hpp"
#include "TypeDescriptor.hpp"

// Uncomment the line below to enable debugging
//#define DEBUG 1

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

std::unique_ptr<ExprNode> &InfixExprNode::left() { return _left; }

std::unique_ptr<ExprNode> &InfixExprNode::right() { return _right; }

std::unique_ptr<TypeDescriptor> InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order
	// Traversal of the expression tree.
	auto lValue = left()->evaluate(symTab);

	if( right() == nullptr) { // we have some unary operator on left()
		if(token().isSubtractionOperator()) { // Negation operation
			changeSign(lValue.get());
			return std::move(lValue);
		} else if( token().isNotKeyword() ) { // Boolean Not operation
			return std::move(negate(lValue.get()));
		} else {
				std::cout << "InfixExprNode::evaluate: don't know how to ";
				std:: cout << "evaluate this unary operator\n";
				token().print();
				std::cout << std::endl;
				exit(2);
		}
	} else {
		auto rValue = right()->evaluate(symTab);

		StringDescriptor *lDesc =
			dynamic_cast<StringDescriptor *>(lValue);
		StringDescriptor *rDesc =
			dynamic_cast<StringDescriptor *>(rValue);

		NumberDescriptor *lDesc =
			dynamic_cast<NumberDescriptor *>(lValue.get());
		NumberDescriptor *rDesc =
			dynamic_cast<NumberDescriptor *>(rValue.get());

		if(lDesc != nullptr && rDesc != nullptr) // we have two strings
			return stringOperations(lDesc, rDesc, token());
		else if(lDesc != nullptr && rDesc != nullptr) { // we have two numbers
			if(token().isBooleanOperator())
				return boolOperations(lDesc, rDesc, token());
			else if(token().isRelOp())
				;
			//return relOperations(lDesc, rDesc, token());
			else if(token().isArithmeticOperator())
				;
			//return arithOperations(lDesc, rDesc, token());
		} else {
			std::cout << "InfixExprNode::evaluate invalid cast\n";
			exit(1);
		}
	}
}

// Boolean operators supporting short circuit logic
		if( token().isAndKeyword() )
			;
		else if( token().isOrKeyword() )
			;		
// Comparison Operators
		if ( token().isEqual() )
			;
		else if ( token().isNotEqual() )
			;
		else if ( token().isLessThan() )
			;
		else if ( token().isGreaterThan() )
			;
		else if ( token().isLessThanEqual() )
			;
		else if ( token().isGreaterThanEqual() )
			;
// Arithmetic Operators
		if( token().isAdditionOperator() )
			;
		else if(token().isSubtractionOperator())
			;
		else if(token().isMultiplicationOperator())
			;
		else if(token().isDivisionOperator())
			;
		else if(token().isFloorDivision())
			;
		else if( token().isModuloOperator() )
			;
		else {
			std::cout << "InfixExprNode::evaluate: don't know how to ";
			std:: cout << "evaluate this operator\n";
			token().print();
			std::cout << std::endl;
			exit(2);
		}	  
	}
//}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}


// WholeNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

std::unique_ptr<TypeDescriptor> WholeNumber::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
#endif
	std::unique_ptr<NumberDescriptor> desc =
		std::make_unique<NumberDescriptor>(TypeDescriptor::INTEGER);
	desc->value.intValue = token().getWholeNumber();
    return desc;
}

// Float
Float::Float(Token token): ExprNode{token} {}

void Float::print() {
	token().print();
}

std::unique_ptr<TypeDescriptor> Float::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "Float::evaluate: returning " << token().getFloat() << std::endl;
#endif
	std::unique_ptr<NumberDescriptor> desc =
		std::make_unique<NumberDescriptor>(TypeDescriptor::INTEGER);
	desc->value.doubleValue = token().getFloat();
    return desc;
}

// Variable
Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

std::unique_ptr<TypeDescriptor> Variable::evaluate(SymTab &symTab) {
    if( !symTab.isDefined( token().getName() )) {
        std::cout << "Use of undefined variable, ";
		std::cout << token().getName() << std::endl;
        exit(1);
    }
#ifdef DEBUG
    std::cout << "Variable::evaluate: returning ";
	printValue( symTab.getValueFor(token().getName()) );
	std::cout << std::endl;
#endif
    return symTab.getValueFor(token().getName());
}

//String
String::String(Token token): ExprNode{token} {}

void String::print() {
	token().print();
}

std::unique_ptr<TypeDescriptor> String::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "String::evaluate: returning " << token().getString() << std::endl;
#endif
	std::unique_ptr<StringDescriptor> desc =
		std::make_unique<StringDescriptor>(TypeDescriptor::STRING);
	desc->value = token().getString();
    return desc;
}
