/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef __PARSER_HPP
#define __PARSER_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <map>

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"

class Parser {
public:
    explicit Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

	std::unique_ptr<Statements> statements();
	std::unique_ptr<AssignmentStatement> assignStatement();
	std::unique_ptr<PrintStatement> printStatement();
	std::unique_ptr<ForStatement> forStatement();
	std::unique_ptr<IfStatement> ifStatement();
	std::unique_ptr<Statements> suite();
	std::vector< std::shared_ptr<ExprNode> >testlist();
	std::unique_ptr<ExprNode> test();
	std::unique_ptr<ExprNode> and_test();
	std::unique_ptr<ExprNode> not_test();
	std::unique_ptr<ExprNode> comparison();
    std::unique_ptr<ExprNode> arith_expr();
    std::unique_ptr<ExprNode> term();
	std::unique_ptr<ExprNode> factor();
    std::unique_ptr<ExprNode> atom();

private:
    Tokenizer &tokenizer;
    void die(std::string const &where, std::string const &message, Token &token);
};

#endif
