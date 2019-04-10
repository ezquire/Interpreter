//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include <vector>
#include "ArithExpr.hpp"
#include "TypeDescriptor.hpp"
#include <cmath>

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }


int InfixExprNode::Intevaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.

    if (token().isArithmeticOperator() )
    {

        int lValue = left()->Intevaluate(symTab);
        int rValue = right()->Intevaluate(symTab);

        if (token().isArithmeticOperator())
            std::cout << "InfixExprNode::Intevaluate: " << lValue << " " << token().symbol() << " " << rValue
                      << std::endl;

        if (token().isAdditionOperator())
            return lValue + rValue;
        else if (token().isSubtractionOperator())
            return lValue - rValue;
        else if (token().isMultiplicationOperator())
            return lValue * rValue;
        else if (token().isDivisionOperator())
            return lValue / rValue; // division by zero?
        else if (token().isModuloOperator())
            return lValue % rValue;
        else if (token().isIntDiv())
            return floor(lValue / rValue);
        else {
            std::cout << "InfixExprNode::Intevaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    else // Relational Op
    {

        TypeDescriptor* descriptor;
        auto lhsName = left()->token().getName();

        // NOTE TO SELF: IMPLEMENT TYPE CHECK VIA TYPEDESCRIPTOR TYPE(), SAVES TIME...

        if (symTab.isDefined(lhsName))
            descriptor = new TypeDescriptor(TypeDescriptor::INTEGER);
        else
            descriptor = symTab.getValueFor(left()->token().getName());

        if (descriptor->type() == TypeDescriptor::STRING)
        {
            std::string lValue = left()->Strevaluate(symTab);
            std::string rValue = right()->Strevaluate(symTab);

            if (token().isGreaterOperator())
                return (lValue > rValue);
            else if (token().isLessOperator())
                return (lValue < rValue);
            else if (token().isGreatEqualOperator())
                return (lValue >= rValue);
            else if (token().isLessEqualOperator())
                return (lValue <= rValue);
            else if (token().isEqualtoOperator())
                return (lValue == rValue);
            else  //(token.isNotEqualOperator())
                return (lValue != rValue);
        }
        else
        {
            int lValue = left()->Intevaluate(symTab);
            int rValue = right()->Intevaluate(symTab);

            if (token().isGreaterOperator())
                return (lValue > rValue);
            else if (token().isLessOperator())
                return (lValue < rValue);
            else if (token().isGreatEqualOperator())
                return (lValue >= rValue);
            else if (token().isLessEqualOperator())
                return (lValue <= rValue);
            else if (token().isEqualtoOperator())
                return (lValue == rValue);
            else  //(token.isNotEqualOperator())
                return (lValue != rValue);
        }

    }
}

std::string InfixExprNode::Strevaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    std::string lValue = left()->Strevaluate(symTab);
    std::string rValue = right()->Strevaluate(symTab);

    if (token().isArithmeticOperator() && token().isAdditionOperator()) {
        std::cout << "InfixExprNode::Strevaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        return lValue + rValue;
    }
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

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::Intevaluate(SymTab &symTab) {
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

// String
String::String(Token token) : ExprNode{token} {}

void String::print() {
    token().print();
}

std::string String::Strevaluate(SymTab &symTab) {
    std::cout << "String::Strevaluate: returning " << token().getStr() << std::endl;
    return token().getStr();
}


// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::Intevaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    TypeDescriptor* TokDesc = symTab.getValueFor(token().getName());
    NumberDescriptor* Toknum = dynamic_cast<NumberDescriptor*>( TokDesc );
    if (Toknum == nullptr) {
        std::cout << "Variable:Inevaluate: evaluation of symbol is of Type STRING, invalid syntax\n";
        exit(1);
    }
    // The variable was an integer
    int retVal = Toknum->value.intValue;
    std::cout << "Variable::evaluate: returning " <<  retVal << std::endl;
    return retVal;

    //std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    //return symTab.getValueFor(token().getName());
}

std::string Variable::Strevaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }

    TypeDescriptor* TokDesc = symTab.getValueFor(token().getName());
    StringDescriptor* Toknum = dynamic_cast<StringDescriptor*>( TokDesc );
    if (Toknum == nullptr) {
        std::cout << "Variable:Strevaluate: evaluation of symbol is of Type NUMBER, invalid syntax\n";
        exit(1);
    }
    // The variable was a String
    std::string retVal = Toknum->strVal();
    std::cout << "Variable::evaluate: returning " <<  retVal << std::endl;
    return retVal;

}

Range::Range(Token token, std::vector<ExprNode *> *iter) : ExprNode{token}, _iterator{iter} {}

void Range::print() {

    for (auto s: *_iterator)
        s->print();

}


int Range::Intevaluate(SymTab &symTab) {

    int ret = 0;
    if (_start < _end)
    {
        symTab.setValueFor(token().getName(), _start += _incr);
        ret++;
    }
    else
        symTab.setValueFor(token().getName(), _start - 1);

    return ret;

}

void Range::initalizeIter(SymTab &symTab) {

    int size = _iterator->size();

    if (size == 1)
    {// 1 parameter
        _start = 0;
        _incr = 1;
        _end = _iterator[0][0]->Intevaluate(symTab);
    }
    else if (size == 2)
    { // 2 parameters
        _start = _iterator[0][0]->Intevaluate(symTab);
        _incr = 1;
        _end = _iterator[0][1]->Intevaluate(symTab);

    }
    else if (size == 3)
    { // 3 parameters
        _start = _iterator[0][0]->Intevaluate(symTab);
        _incr = _iterator[0][1]->Intevaluate(symTab);
        _end = _iterator[0][2]->Intevaluate(symTab);
    }
    else {
        std::cout << "ArithExpr:Range::initalizeIter - Range can only have at most 3 parameters, given -> " << size << " <- parameters";
        exit(1);
    }

    auto name = token().getName();
    symTab.setValueFor(name, _start);

}


