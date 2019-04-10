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

    bool isColon() const { return _symbol == ':';}
    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
    bool isOpenBrack() const  { return _symbol == '{'; }
    bool isCloseBrack() const { return _symbol == '}'; }
    bool isComma() const { return _symbol == ',';}
    bool isIn() const { return _keyword == "in";}
    bool isRange() const { return _keyword == "range";}

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
    void Relsymbol(std::string c) { _Relsymbol = c; }
    std::string Get_Relsymbol() const { return _Relsymbol; }

    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isIntDiv() const { return _Relsymbol == "//";}
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator() ||
               isIntDiv();
    }
    bool isGreaterOperator() const { return _Relsymbol == ">"; }
    bool isLessOperator() const { return _Relsymbol == "<"; }
    bool isEqualtoOperator() const { return _Relsymbol == "=="; }
    bool isGreatEqualOperator() const { return _Relsymbol == ">="; }
    bool isLessEqualOperator() const { return _Relsymbol == "<="; }
    bool isNotEqualOperator() const { return _Relsymbol == "!="; }
    bool isRelationalOperator() const {
        return _Relsymbol != "" &&
               ( isGreaterOperator() ||
                 isLessOperator() ||
                 isEqualtoOperator() ||
                 isGreatEqualOperator() ||
                 isLessEqualOperator() ||
                 isNotEqualOperator() );
    }

    bool isKeyWord() const { return ((_keyword == "for" || _keyword == "print" || _keyword == "if" || _keyword == "elif" || _keyword == "else") ? true : false);}
    bool isStr() const { return _string.length() > 0;}
    void setKeyWord(std::string c) {_keyword = c;}
    bool isPrint() const {return (_keyword == "print");}
    bool isFor() const { return (_keyword == "for"); }
    bool isIf() const { return _keyword == "if";}
    bool isElif() const { return _keyword == "elif";}
    bool isElse() const { return _keyword == "else";}
    bool isOr() const { return _keyword == "or";}
    bool isAnd() const { return _keyword == "and";}
    bool isNot() const { return _keyword == "not";}

    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
    void setStr(std::string s) { _string = s; }
    std::string getStr() const { return _string;}

    void print() const;

    void Indent() {_isIndent = true;}
    void Dedent() {_isDedent = true;}
    bool isIndent() const { return _isIndent;}
    bool isDedent() const { return _isDedent;}


private:
    std::string _name;
    std::string _keyword;
    std::string _Relsymbol;
    std::string _string;
    bool _eof, _eol;
    bool _isWholeNumber;
    bool _isIndent;
    bool _isDedent;
    char _symbol;
    int _wholeNumber;
};

#endif //EXPRINTER_TOKEN_HPP
