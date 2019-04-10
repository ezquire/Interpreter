//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include <stack>
#include "Token.hpp"
#include "SymTab.hpp"
class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();
    int getIndentationLvl() {return _indent.size();}

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens;
    bool parssingANewLine = true;
    std::stack<int> _indent; 
    int spaceCounter = 0;

private:
    void readComment();
    std::string readString();
    std::string readName();
    int readInteger();
    std::string readEquiv();
    std::string readUnequiv();
    std::string readGreater();
    std::string readLess();
};

#endif //EXPRINTER_TOKENIZER_HPP
