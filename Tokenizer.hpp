//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"
#include <stack>

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
  void printProcessedTokens();

private:
    Token lastToken;
    bool ungottenToken;
    bool parsingNewLine;
    std::ifstream &inStream;
    std::vector<Token> _tokens;

private:
    std::string readName();
    int readInteger();
    std::string readString();
    std::stack<int> Stack;
};

#endif //EXPRINTER_TOKENIZER_HPP
