//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _string{""},_indent{false},_dedent{false}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( isComma()) std::cout<< ",";
    else if( eof() )                    std::cout << "EOF" ;
    else if( indent()) std::cout<<"INDENT";
    else if( isQuote()) std::cout<< '"' ;
    else if( dedent ()) std::cout<<"DEDENT";
    else if( isKeywordPrint()) std::cout<<"print";
    else if( isKeywordFor()) std::cout<<"for";
    else if( isKeywordRange()) std::cout<<"range";
    else if( isKeywordIf()) std::cout<<"if";
    else if( isOrOperator ()) std::cout<<"or";
    else if( isAndOperator()) std::cout<<"and";
    else if( isNotOperator()) std::cout<<"not";
    else if( isOpenBrace() ) std::cout<< "{" ;
    else if( isCloseBrace() ) std::cout<<"}";
    else if( isEquivalentOperator() ) std::cout << "==" ;
    else if( isUnequivalentOperator() ) std::cout << "!=" ;
    else if( isGreaterThanOperator() ) std::cout << ">" ;
    else if( isLessThanOperator() ) std::cout << "<" ;
    else if( isGreaterThanOrEqualOperator() ) std::cout << ">=" ;
    else if( isLessThanOrEqualOperator() ) std::cout << "<=" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isColon()) std::cout<<":";
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isString() ) std::cout << getString();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else std::cout << "Uninitialized token.\n";
}
