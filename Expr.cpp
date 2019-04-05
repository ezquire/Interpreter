// Created by Tyler Gearing 4/4/2019

#include <iostream>
#include <cmath>
#include "Expr.hpp"

// Uncomment the line below to enable debugging
//#define DEBUG 1

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order
	// Traversal of the expression tree.
    auto lValue = left()->evaluate(symTab);

	if( right() == nullptr) { // we have some unary operator on left()
		if(token().isSubtractionOperator() ) { // Negation operation
			NumberDescriptor *lDesc = dynamic_cast<NumberDescriptor *>(lValue);
			if( lDesc == nullptr) {
				std::cout << "Error, failed to cast TypeDescriptor\n";
				exit(1);
			} else if(lDesc->type() == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue *= -1;
			else if(lDesc->type() == TypeDescriptor::INTEGER)
				lDesc->value.intValue *= -1;
			return lDesc;
		} else if( token().isNotKeyword() ) { // Boolean Not operation
			NumberDescriptor *lDesc = dynamic_cast<NumberDescriptor *>(lValue);
			if( lDesc == nullptr) {
				std::cout << "Error, failed to cast TypeDescriptor\n";
				exit(1);
			} else if (lDesc->type() == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue == 1)
					lDesc->value.boolValue = 0;
				else lDesc->value.boolValue = 1;
				return lDesc;
			} else if (lDesc->type() == TypeDescriptor::INTEGER) {
				lDesc->type() == TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue == 0)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
				return lDesc;
			} else if(lDesc->type() == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue == 0)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
				return lDesc;
			}
		}
	}

	auto rValue = right()->evaluate(symTab);

	StringDescriptor *lDesc = dynamic_cast<StringDescriptor *>(lValue);
	StringDescriptor *rDesc = dynamic_cast<StringDescriptor *>(rValue);

	if(lDesc != nullptr && rDesc != nullptr) { // we have two strings
		if( token().isAdditionOperator() ) {
			lDesc->value += rDesc->value;
			return lDesc;
		} else if ( token().isEqual() ) {
			NumberDescriptor * ret =
				new NumberDescriptor(TypeDescriptor::BOOLEAN);
			if( lDesc->value == rDesc->value )
				ret->value.boolValue = 1;
			else ret->value.boolValue = 0;
			return ret;
		} else if ( token().isNotEqual() ) {
			NumberDescriptor * ret =
				new NumberDescriptor(TypeDescriptor::BOOLEAN);
			if( lDesc->value != rDesc->value )
				ret->value.boolValue = 1;
			else ret->value.boolValue = 0;
			return ret;
		}		
	} else { // we have two numbers
		NumberDescriptor *lDesc = dynamic_cast<NumberDescriptor *>(lValue);
		NumberDescriptor *rDesc = dynamic_cast<NumberDescriptor *>(rValue);
		if(lDesc == nullptr && rDesc == nullptr) {
			std::cout << "Unable to cast TypeDescriptor to NumberDescriptor";
			exit(1);
		}
		auto lType = lDesc->type();
		auto rType = rDesc->type();			

// Boolean operators supporting short circuit logic
		if( token().isAndKeyword() ) {
			if(lType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue == 0)
					return lDesc;
				else return rDesc;
			} else if(lType == TypeDescriptor::DOUBLE) {
				if(lDesc->value.doubleValue == 0.0)
					return lDesc;
				else return rDesc;
			} else if(lType == TypeDescriptor::INTEGER) {
				if(lDesc->value.intValue == 0)
					return lDesc;
				else return rDesc;
			}
		} else if( token().isOrKeyword() ) {
			if(lType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue != 0)
					return lDesc;
				else return rDesc;
			} else if(lType == TypeDescriptor::DOUBLE) {
				if(lDesc->value.doubleValue != 0.0)
					return lDesc;
				else return rDesc;
			} else if(lType == TypeDescriptor::INTEGER) {
				if(lDesc->value.intValue != 0)
					return lDesc;
				else return rDesc;
			}
		}
		
// Comparison Operators
		if ( token().isEqual() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue == rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue == rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue == rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue == rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue == rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue == rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue == rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue == rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue == rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		} else if ( token().isNotEqual() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue != rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue != rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue != rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue != rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue != rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue != rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue != rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue != rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue != rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		} else if ( token().isLessThan() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue < rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue < rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue < rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue < rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue < rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue < rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue < rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue < rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue < rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		} else if ( token().isGreaterThan() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue > rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue > rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue > rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue > rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue > rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue > rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue > rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue > rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue > rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		} else if ( token().isLessThanEqual() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue <= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue <= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue <= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue <= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue <= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue <= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue <= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue <= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue <= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		} else if ( token().isGreaterThanEqual() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue >= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue >= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.doubleValue >= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue >= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue >= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::BOOLEAN) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.intValue >= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::BOOLEAN) {
				if(lDesc->value.boolValue >= rDesc->value.boolValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::BOOLEAN;
				if(lDesc->value.boolValue >= rDesc->value.doubleValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			} else if(lType == TypeDescriptor::BOOLEAN &&
					  rType == TypeDescriptor::INTEGER) {
				if(lDesc->value.boolValue >= rDesc->value.intValue)
					lDesc->value.boolValue = 1;
				else lDesc->value.boolValue = 0;
			}
			return lDesc;
		}

// Arithmetic Operators
		if( token().isAdditionOperator() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue += rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.doubleValue += rDesc->value.intValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.doubleValue += rDesc->value.boolValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue += rDesc->value.intValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::DOUBLE;
				lDesc->value.doubleValue += rDesc->value.intValue;
			} else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue += rDesc->value.boolValue;
			return lDesc;
		} else if(token().isSubtractionOperator() && right() != nullptr) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue -= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.doubleValue -= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.doubleValue -= rDesc->value.boolValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue -= rDesc->value.intValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::DOUBLE)
				lDesc->value.intValue -= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue -= rDesc->value.boolValue;
			return lDesc;
		} else if(token().isMultiplicationOperator()) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue *= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.doubleValue *= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.doubleValue *= rDesc->value.boolValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue *= rDesc->value.intValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::DOUBLE)
				lDesc->value.intValue *= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue *= rDesc->value.boolValue;
			return lDesc;
		} else if(token().isDivisionOperator()) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue /= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.doubleValue /= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.doubleValue /= rDesc->value.boolValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue /= rDesc->value.intValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::DOUBLE)
				lDesc->value.intValue /= rDesc->value.doubleValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue /= rDesc->value.boolValue;
			return lDesc;
		} else if(token().isFloorDivision()) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE)
				lDesc->value.doubleValue =
					floor(lDesc->value.doubleValue / rDesc->value.doubleValue);
			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.doubleValue =
					floor(lDesc->value.doubleValue / rDesc->value.intValue);
 			else if(lType == TypeDescriptor::DOUBLE &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.doubleValue =
				floor(lDesc->value.doubleValue / rDesc->value.boolValue);
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue =
					floor(lDesc->value.intValue / rDesc->value.intValue);
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::DOUBLE) {
				lDesc->type() = TypeDescriptor::DOUBLE;
				lDesc->value.doubleValue =
					floor(lDesc->value.intValue / rDesc->value.doubleValue);
			} else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue =
					floor(lDesc->value.intValue / rDesc->value.boolValue);
			return lDesc;
		} else if( token().isModuloOperator() ) {
			if(lType == TypeDescriptor::DOUBLE &&
			   rType == TypeDescriptor::DOUBLE) {
				std::cout << "Invalid operands for operator %. ";
				std::cout << "Types: double and double\n";
				exit(2);
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::INTEGER) {
				std::cout << "Invalid operands for operator %. ";
				std::cout << "Types: double and integer\n";
				exit(2);
			} else if(lType == TypeDescriptor::DOUBLE &&
					  rType == TypeDescriptor::BOOLEAN) {
				std::cout << "Invalid operands for operator %. ";
				std::cout << "Types: double and boolean\n";
				exit(2);
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::DOUBLE) {
				std::cout << "Invalid operands for operator %. ";
				std::cout << "Types: integer and double\n";
				exit(2);
			} else if(lType == TypeDescriptor::INTEGER &&
					  rType == TypeDescriptor::INTEGER)
				lDesc->value.intValue %= rDesc->value.intValue;
			else if(lType == TypeDescriptor::INTEGER &&
					rType == TypeDescriptor::BOOLEAN)
				lDesc->value.intValue %= rDesc->value.boolValue;
			return lDesc;
		} else {
			std::cout << "InfixExprNode::evaluate: don't know how to ";
			std:: cout << "evaluate this operator\n";
			token().print();
			std::cout << std::endl;
			exit(2);
		}	  
	}
}

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

TypeDescriptor *WholeNumber::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
#endif
	NumberDescriptor * desc = new NumberDescriptor(TypeDescriptor::INTEGER);
	desc->value.intValue = token().getWholeNumber();
    return desc;
}

// Float
Float::Float(Token token): ExprNode{token} {}

void Float::print() {
	token().print();
}

TypeDescriptor *Float::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "Float::evaluate: returning " << token().getFloat() << std::endl;
#endif
	NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
	desc->value.doubleValue = token().getFloat();
    return desc;
}

// Variable
Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
    if( !symTab.isDefined( token().getName() ) ) {
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

TypeDescriptor *String::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "String::evaluate: returning " << token().getString() << std::endl;
#endif
	StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
	desc->value = token().getString();
    return desc;
}
