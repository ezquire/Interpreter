//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }
    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

	bool &indent()  { return _indent; }
    bool &dedent()  { return _dedent; }
    bool indent() const { return _indent; }
    bool dedent() const  { return _dedent; }
	
    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
	bool isOpenBrace() const  { return _symbol == '{'; }
    bool isCloseBrace() const { return _symbol == '}'; }

	bool isPrintKeyword() { return _name == "print"; }
	bool isForKeyword() { return _name == "for"; }

	bool isName() const { return _name.length() > 0; }

	bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
	
	bool &isFloat() { return _isFloat; }
    bool isFloat() const { return _isFloat; }

	bool &isBool() { return _isBool; }
    bool isBool() const { return _isBool; }

    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }

	bool isEqual() const { return _relOp == "=="; }
	bool isNotEqual() const { return _relOp == "!=" || _relOp == "<>"; }
	bool isLessThan() const { return _relOp == "<"; }
	bool isGreaterThan() const { return _relOp == ">"; }
	bool isLessThanEqual() const { return _relOp == "<="; }
	bool isGreaterThanEqual() const { return _relOp == ">="; }
	bool isRelOp() const {
		return isEqual() ||
			isNotEqual() ||
			isLessThan() ||
			isGreaterThan() ||
			isLessThanEqual() ||
			isGreaterThanEqual();
	}

	void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
	
	void relOp(std::string op) { _relOp = op; }
	std::string relOp() { return _relOp; }

    void setName(std::string n) { _name = n; }
	std::string getName() const { return _name; }

    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void print() const;

private:
    std::string _name;
	std::string _relOp;
    bool _eof, _eol;
	bool _indent, _dedent;
	bool _isWholeNumber;
	bool _isFloat;
	bool _isBool;
    char _symbol;
    int _wholeNumber;
	double _float;
};

#endif //EXPRINTER_TOKEN_HPP
