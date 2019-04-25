/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include <iostream>
#include <memory>
#include <cmath>

#include "Expr.hpp"
#include "TypeDescriptor.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1

// ExprNode
ExprNode::ExprNode(Token token): _token{std::move(token)} {}

Token ExprNode::token() { return _token; }

// InfixExprNode
InfixExprNode::InfixExprNode(Token tk) : ExprNode{std::move(tk)}, _left(nullptr), _right(nullptr) {}

std::unique_ptr<ExprNode> &InfixExprNode::left() { return _left; }

std::unique_ptr<ExprNode> &InfixExprNode::right() { return _right; }

std::shared_ptr<TypeDescriptor> InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order
	// Traversal of the expression tree.
	auto lValue = left()->evaluate(symTab);

	if( right() == nullptr) { // we have some unary operator on left()
		if(token().isSubtractionOperator()) { // Negation operation
			changeSign(lValue.get());
			return lValue;
		} else if( token().isNotKeyword() ) { // Boolean Not operation
			return negate(lValue.get());
		} else {
				std::cout << "InfixExprNode::evaluate: don't know how to ";
				std:: cout << "evaluate this unary operator\n";
				token().print();
				std::cout << std::endl;
				exit(2);
		}
	} else { // We have a left() and a right()
		auto rValue = right()->evaluate(symTab);
		auto slDesc = dynamic_cast<StringDescriptor *>(lValue.get());
		auto srDesc = dynamic_cast<StringDescriptor *>(rValue.get());
		auto lDesc = dynamic_cast<NumberDescriptor *>(lValue.get());
		auto rDesc = dynamic_cast<NumberDescriptor *>(rValue.get());

		if(slDesc != nullptr && srDesc != nullptr) // we have two strings
			return stringOperations(slDesc, srDesc, token());
		else if(lDesc != nullptr && rDesc != nullptr) { // we have two numbers
			if(token().isBooleanOperator()) { // Needs its own function? 
				auto lType = lDesc->type();
				if( token().isAndKeyword() ) {
					if(lType == TypeDescriptor::BOOLEAN) {
						if(lDesc->value.boolValue == 0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::DOUBLE) {
						if(lDesc->value.doubleValue == 0.0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::INTEGER) {
						if(lDesc->value.intValue == 0)
							return lValue;
						else return rValue;
					}
				} else if( token().isOrKeyword() ) {
					if(lType == TypeDescriptor::BOOLEAN) {
						if(lDesc->value.boolValue != 0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::DOUBLE) {
						if(lDesc->value.doubleValue != 0.0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::INTEGER) {
						if(lDesc->value.intValue != 0)
							return lValue;
						else return rValue;
					}
				}
			} else if(token().isRelOp())	
				return relOperations(lDesc, rDesc, token());
			else if(token().isArithmeticOperator())
				return arithOperations(lDesc, rDesc, token());
			else {
				std::cout << "InfixExprNode::evaluate: don't know how to ";
				std:: cout << "evaluate this operator\n";
				token().print();
				std::cout << std::endl;
				exit(2);
			}
		} else {
			std::cout << "InfixExprNode::evaluate invalid cast\n";
			exit(1);
		}
	}
	return nullptr; // Should never hit here
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WholeNumber
WholeNumber::WholeNumber(Token token): ExprNode{std::move(token)} {}

void WholeNumber::print() {
    token().print();
}

std::shared_ptr<TypeDescriptor> WholeNumber::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "WholeNumber::evaluate: returning ";
	std::cout << token().getWholeNumber() << std::endl;
#endif
	std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
	desc->value.intValue = token().getWholeNumber();
    return desc;
}

// Float
Float::Float(Token token): ExprNode{std::move(token)} {}

void Float::print() {
	token().print();
}

std::shared_ptr<TypeDescriptor> Float::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "Float::evaluate: returning ";
	std::cout << token().getFloat() << std::endl;
#endif
	std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::DOUBLE);
	desc->value.doubleValue = token().getFloat();
    return desc;
}

// Variable
Variable::Variable(Token token): ExprNode{std::move(token)} {}

void Variable::print() {
    token().print();
}

std::shared_ptr<TypeDescriptor> Variable::evaluate(SymTab &symTab) {
    if( !symTab.isDefined( token().getName() )) {
        std::cout << "Use of undefined variable, ";
		std::cout << token().getName() << std::endl;
        exit(1);
    }
#ifdef DEBUG
    std::cout << "Variable::evaluate: returning ";
	printValue( symTab.getValueFor(token().getName()).get() );
	std::cout << std::endl;
#endif
    return symTab.getValueFor(token().getName());
}

// String
String::String(Token token): ExprNode{std::move(token)} {}

void String::print() {
	token().print();
}

std::shared_ptr<TypeDescriptor> String::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "String::evaluate: returning " << token().getString() << std::endl;
#endif
	std::shared_ptr<StringDescriptor> desc =
		std::make_shared<StringDescriptor>(TypeDescriptor::STRING);
	desc->value = token().getString();
    return desc;
}

// Call 
