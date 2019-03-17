//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "Expr.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    int lValue = left()->evaluate(symTab);
    int rValue = right()->evaluate(symTab);
#ifdef DEBUG
    std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
#endif
	if ( token().isEqual() )
		return lValue == rValue;
	else if ( token().isNotEqual() )
		return lValue != rValue;
	else if ( token().isLessThan() )
		return lValue < rValue;
	else if ( token().isGreaterThan() )
		return lValue > rValue;
	else if ( token().isLessThanEqual() )
		return lValue <= rValue;
	else if ( token().isGreaterThanEqual() )
		return lValue >= rValue;
    else if( token().isAdditionOperator() )
        return lValue + rValue;
    else if(token().isSubtractionOperator())
        return lValue - rValue;
    else if(token().isMultiplicationOperator())
        return lValue * rValue;
    else if(token().isDivisionOperator())
        return lValue / rValue; // division by zero?
    else if( token().isModuloOperator() )
        return lValue % rValue;
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
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

int WholeNumber::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
#endif
    return token().getWholeNumber();
}

// Float
Float::Float(Token token): ExprNode{token} {}

void Float::print() {
	token().print();
}

int Float::evaluate(SymTab &symTab) {
#ifdef DEBUG
    std::cout << "Float::evaluate: returning " << token().getFloat() << std::endl;
#endif
    return token().getFloat();
}

// Variable
Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::evaluate(SymTab &symTab) {
    if( !symTab.isDefined( token().getName() ) ) {
        std::cout << "Use of undefined variable, ";
		std::cout << token().getName() << std::endl;
        exit(1);
    }
#ifdef DEBUG
    std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
#endif
    return symTab.getValueFor(token().getName());
}


