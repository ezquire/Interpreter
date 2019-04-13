/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include <stack>
#include "Token.hpp"

class Tokenizer {
public:
    explicit Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
	void printProcessedTokens();
	bool getBol() const { return bol; }
private:
    Token lastToken;
	int col;
    bool ungottenToken;
	bool bol;
	std::stack<int> stack;
    std::ifstream &inStream;
    std::vector<Token> _tokens;
private:
    std::string readName();
	std::string readOp();
	std::string readString();
    void readNumber(Token &tok);
};

#endif //EXPRINTER_TOKENIZER_HPP
