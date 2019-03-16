//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _indent{false}, _dedent{false}, _symbol{'\0'}, _isWholeNumber{false} {}

void Token::print() const {
    if( eol() ) std::cout << "NEWLINE" ;
    else if( eof() ) std::cout << "EOF" ;
	else if( indent() ) std::cout << "INDENT";
	else if( dedent() ) std::cout << "DEDENT";
	else if( isOpenBrace() ) std::cout << " { ";
	else if( isCloseBrace() ) std::cout << " } ";
	else if( isEqual() ) std::cout << " == ";
	else if( isNotEqual() ) std::cout << " != ";
	else if( isLessThan() ) std::cout << " < ";
	else if( isGreaterThan() ) std::cout << " > ";
	else if( isLessThanEqual() ) std::cout << " <= ";
	else if( isGreaterThanEqual() ) std::cout << " >= ";
    else if( isOpenParen() )  std::cout << " ( " ;
    else if( isCloseParen() )  std::cout << " ) " ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else std::cout << "Uninitialized token.\n";
}
