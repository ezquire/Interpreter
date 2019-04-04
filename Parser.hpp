
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();
    AssignmentStatement *assignStatement();
	PrintStatement *printStatement();
	ForStatement *forStatement();
	IfStatement *ifStatement();

	Statements *suite();
	
	std::vector<ExprNode *>testlist();
	ExprNode *test();
	ExprNode *and_test();
	ExprNode *not_test();
	ExprNode *comparison();
    ExprNode *arith_expr();
    ExprNode *term();
	ExprNode *factor();
    ExprNode *atom();

    //std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

