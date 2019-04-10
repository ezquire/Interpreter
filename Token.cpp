//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _isIndent{false}, _isDedent{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if ( isGreaterOperator() ) { std::cout <<  " > "; }
    else if ( isLessOperator() ) { std::cout <<  " < "; }
    else if ( isEqualtoOperator() ) { std::cout <<  " == "; }
    else if ( isGreatEqualOperator() ) { std::cout <<  " >= "; }
    else if ( isLessEqualOperator() ) { std::cout <<  " <= "; }
    else if ( isNotEqualOperator() ) { std::cout <<  " != "; }
    else if ( isKeyWord() ) std::cout << " " << _keyword << " ";
    else if ( isOpenBrack() ) std::cout << " " << "{" << " ";
    else if ( isCloseBrack() ) std::cout << " " << "}" << " ";
    else if ( isStr() ) std::cout << getStr();
    else if ( isIndent() ) std::cout << "Indent Token";
    else if ( isDedent() ) std::cout << "Dedent Token";
    else if ( isComma() ) std::cout << ", ";
    else if ( isIntDiv() ) std::cout << " // ";
    else if ( isIn() ) std::cout << " in ";
    else if ( isRange() ) std::cout << " range ";
    else if ( isIf() ) std::cout << " if ";
    else if ( isElif() ) std::cout << " elif ";
    else if ( isElse() ) std::cout << " else ";
    else if ( isColon() ) std::cout << " : ";
    else if ( isAnd() ) std::cout << " and ";
    else if ( isOr() ) std::cout << " or ";
    else if ( isNot() ) std::cout << " not ";
    else std::cout << "Uninitialized token.\n";
}


