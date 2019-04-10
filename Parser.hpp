
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ArithExpr.hpp"
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

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();
    ExprNode *rel_expr();
    ExprNode *rel_term();
    std::vector<ExprNode*> testList();
    ExprNode *comparison();
    ExprNode *not_test();
    ExprNode *and_test();
    ExprNode *or_test();
    ExprNode *test();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

