/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP

#include <string>
#include <memory>
#include <iostream>

class Token {

public:
    Token();

	// Special Tokens
    bool &eof()         { return _eof;    }
    bool eof() const    { return _eof;    }
	bool &eol()         { return _eol;    }
    bool eol() const    { return _eol;    }
	bool &indent()      { return _indent; }
    bool indent() const { return _indent; }
	bool &dedent()      { return _dedent; }
    bool dedent() const { return _dedent; }

	// Special Keywords
	bool isPrintKeyword() const  { return _keyword == "print";  }
	bool isForKeyword() const    { return _keyword == "for";    }
	bool isInKeyword() const     { return _keyword == "in";     }
	bool isRangeKeyword() const  { return _keyword == "range";  }
	bool isReturnKeyword() const { return _keyword == "return"; }
	bool isDefKeyword() const    { return _keyword == "def";    }
	bool isLenKeyword() const    { return _keyword == "len";    }
    bool isPopKeyword() const    { return _keyword == "pop"; }
    bool isAppendKeyword() const { return _keyword == "append"; }

	// Boolean Keywords
	bool isOrKeyword() const  { return _keyword == "or";  }
	bool isAndKeyword() const { return _keyword == "and"; }
	bool isNotKeyword() const { return _keyword == "not"; }
	bool isBooleanOperator() const {
		return isOrKeyword() ||
			isAndKeyword() ||
			isNotKeyword();
	}
	// Control Flow
	bool isIf() const   { return _keyword == "if";   }
	bool isElif() const { return _keyword == "elif"; }
	bool isElse() const { return _keyword == "else"; }

	// Functions
	bool &isCall()         { return _call;    }
  	bool isCall() const    { return _call;    }
	bool &isSub()          { return _sub;     }
	bool isSub() const     { return _sub;     }
	bool isArrayOp() const { return _arrayOp.length() > 0; }
	bool isAppend() const  { return _arrayOp == "append";  }
	bool isPop() const     { return _arrayOp == "pop";     }

	bool isName() const {
		return _name.length() > 0 && !isArrayOp() && !isCall() && !isSub();
	}

	// Statements
	bool isSimpleStatement() const {
		return isPrintKeyword() || isName() || isArrayOp() || isCall() ||
			   isReturnKeyword();
	}
	bool isCompoundStatement() const { return isForKeyword() || isIf(); }
	
	// Types
	bool isString() const      { return _string.length() > 0; }
	bool &isWholeNumber()      { return _isWholeNumber;       }
    bool isWholeNumber() const { return _isWholeNumber;       }
	bool &isFloat()            { return _isFloat;             }
    bool isFloat() const       { return _isFloat;             }	
	bool isKeyword() const     { return _keyword.length() > 0;}
   
	// Assignment Operator
	bool isAssignmentOperator() const { return _symbol == '='; }

	// Sentinal Characters
    bool isColon() const      { return _symbol == ':'; }
	bool isComma() const      { return _symbol == ','; }
    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
	bool isOpenBrace() const  { return _symbol == '{'; }
    bool isCloseBrace() const { return _symbol == '}'; }
    bool isOpenBrack() const  { return _symbol == '['; }
    bool isCloseBrack() const { return _symbol == ']'; }
    bool isPeriod() const     { return _symbol == '.';}

	// Arithmetic Operators
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
	bool isFloorDivision() const          { return _relOp == "//"; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
			isAdditionOperator() ||
			isSubtractionOperator() ||
			isModuloOperator() ||
			isDivisionOperator() ||
			isFloorDivision();
    }
	
	// Relational Operators
	bool isEqual() const            { return _relOp == "=="; }
	bool isNotEqual() const         { return _relOp == "!=" || _relOp == "<>";}
	bool isLessThan() const         { return _relOp == "<";  }
	bool isGreaterThan() const      { return _relOp == ">";  }
	bool isLessThanEqual() const    { return _relOp == "<="; }
	bool isGreaterThanEqual() const { return _relOp == ">="; }
	bool isRelOp() const {
		return isEqual() ||
			isNotEqual() ||
			isLessThan() ||
			isGreaterThan() ||
			isLessThanEqual() ||
			isGreaterThanEqual();
	}

	// Set and Get Functions
	void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
	
	void relOp(std::string op) { _relOp = std::move(op); }
	std::string relOp() { return _relOp; }

    void setName(std::string n) { _name = std::move(n); }
	std::string getName() const { return _name; }

	void setKeyword(std::string k) { _keyword = std::move(k); }
	std::string getKeyword() const { return _keyword; }

	void setString(std::string n) { _string = std::move(n); }
	std::string getString() const { return _string; }

	void setArrayOp(std::string op) { _arrayOp = std::move(op); }
	std::string getArrayOp() const  { return _arrayOp; }

    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
    int getWholeNumber() const { return _wholeNumber; }

	void setFloat(double n) {
		_float = n;
		isFloat() = true;
	}
	double getFloat() const { return _float; }

	// Print Function
    void print() const;

private:
  std::string _name;
	std::string _string;
	std::string _relOp;
	std::string _keyword;
	std::string _arrayOp;
  bool _eof, _eol;
	bool _indent, _dedent;
	bool _isWholeNumber, _isFloat;
	bool _call, _sub;
  char _symbol;
  int _wholeNumber;
	double _float;
};

#endif //EXPRINTER_TOKEN_HPP
