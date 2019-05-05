/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Token.hpp"

Token::Token(): _name{""},
				_string{""},
				_relOp{""},
				_keyword{""},
				_arrayOp{""},
				_eof{false},
				_eol{false},
        _indent{false},
				_dedent{false},
        _isWholeNumber{false},
				_isFloat{false},
				_call{false},
        _sub{false},
        _symbol{'\0'},
        _wholeNumber{0},
				_float{0.0} {}

void Token::print() const {
    if( eol() ) std::cout << "NEWLINE" ;
    else if( eof() ) std::cout << "ENDMARKER" ;
    else if( indent() ) std::cout << "INDENT";
    else if( dedent() ) std::cout << "DEDENT";
    else if( isOpenBrace() ) std::cout << " { ";
    else if( isCloseBrace() ) std::cout << " } ";
    else if( isComma() ) std::cout << " , ";
    else if( isPeriod()) std::cout<< ".";
    else if( isEqual() ) std::cout << " == ";
    else if( isNotEqual() ) std::cout << " != ";
    else if( isLessThan() ) std::cout << " < ";
    else if( isGreaterThan() ) std::cout << " > ";
    else if( isLessThanEqual() ) std::cout << " <= ";
    else if( isGreaterThanEqual() ) std::cout << " >= ";
    else if( isOpenParen() )  std::cout << " ( " ;
    else if( isCloseParen() )  std::cout << " ) " ;
    else if( isAssignmentOperator() )  std::cout << " = ";
    else if( isColon() )  std::cout << " : " ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + ";
    else if( isSubtractionOperator() )  std::cout << " - ";
    else if( isModuloOperator() )  std::cout << " % ";
    else if( isDivisionOperator() )  std::cout << " / ";
    else if( isFloorDivision() ) std::cout << " // ";
    else if( isOpenBrack() ) std::cout<< "[";
    else if( isCloseBrack() ) std::cout<< "]";
    else if( isName() )  std::cout << getName();
	  else if( isKeyword() ) std::cout << getKeyword();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isFloat() ) std::cout << getFloat();
    else if( isString() ) std::cout << getString();
	  else if( isCall() ) std::cout << "CALL " << getName();
    else if( isSub() ) std::cout << "ARRAY SUB " << getName();
    else if( isAppend() ) std::cout << "ARRAY APPEND " << getName();
	  else if( isPop() ) std::cout << "ARRAY POP " << getName();
    else std::cout << "Uninitialized token.\n";
}
