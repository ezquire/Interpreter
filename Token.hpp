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
    bool &indent() { return _indent;}
    bool &dedent() { return _dedent;}
    
    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }
    bool indent() const { return _indent;}
    bool dedent() const { return _dedent;}
  
  
    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    bool isOpenBrace() const { return _symbol == '{'; }
    bool isCloseBrace() const { return _symbol == '}'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }

    void setString(std::string s) { _string = s; }
    std::string getString() const { return _string; }

    bool isKeywordPrint() const { return _name == "print"; }
    bool isKeywordFor() const { return _name == "for"; }
    bool isKeywordRange() const { return _name =="range";}
    bool isKeywordIf() const {return _name == "if";}
    bool isOrOperator() const {return _name == "or";}
    bool isAndOperator() const {return _name == "and";}
    bool isNotOperator() const {return _name == "not";}

    bool isComma()              const { return _symbol == ','; }
    bool isEquivalentOperator() const { return _equiv == "=="; }
    bool isUnequivalentOperator() const { return _equiv == "!="; }
    bool isGreaterThanOperator() const { return _symbol == '>'; }
    bool isLessThanOperator() const { return _symbol == '<'; }
    bool isGreaterThanOrEqualOperator() const { return _equiv == ">="; }
    bool isLessThanOrEqualOperator() const { return _equiv == "<="; }
    bool isSemiColon() const { return _symbol == ';'; }
    bool isColon() const { return _symbol == ':'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    
    bool isQuote() const                  { return _symbol == '"'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    bool isRelOperator() const {
      return isEquivalentOperator() ||
	isUnequivalentOperator() ||
	isGreaterThanOperator() ||
	isLessThanOperator() ||
	isGreaterThanOrEqualOperator() ||
	isLessThanOrEqualOperator();
    }
    bool isName() const                   { return _name.length() > 0; }
    bool isString() const                  { return _string.length() > 0; }
    std::string getName() const                  { return _name; }
    
    void setName(std::string n) { _name = n; }
    std::string getEquiv() { return _equiv; }
    void setEquiv(std::string n) { _equiv = n; }
    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
  
    void print() const;
  
private:

    //equiv string holds value for relational operators
    //requiring two chars ex: "<="
    std::string _equiv;
    std::string _name;
    std::string _string;
  
    bool _eof, _eol, _indent, _dedent;
    bool _isWholeNumber;
    char _symbol;
    int _wholeNumber;
};

#endif //EXPRINTER_TOKEN_HPP
