/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP

#include "Token.hpp"
#include <stack>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
	void printProcessedTokens();
	bool getBol() const { return bol; }
private:
	bool ungottenToken;
	bool bol;
	int col;
	std::ifstream &inStream;
    Token lastToken;
	std::stack<int> stack;
    std::vector<Token> _tokens;
private:
	bool isKeyword(std::string str);
    std::string readName();
	std::string readOp();
	std::string readString();
    void readNumber(Token &tok);
};

#endif //EXPRINTER_TOKENIZER_HPP
