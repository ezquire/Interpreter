/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Tokenizer.hpp"
#include "AST.hpp"

class Parser {
public:
    explicit Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

	std::unique_ptr<AST> file_input();

	std::unique_ptr<Statement> stmt();
	std::unique_ptr<Statement> simple_stmt();
	std::unique_ptr<PrintStatement> print_stmt();
	std::unique_ptr<AssignmentStatement> assign_stmt();
	std::unique_ptr<Statement> compound_stmt();
	std::unique_ptr<ReturnStatement> return_stmt();
	std::vector<std::shared_ptr<ExprNode>> testlist();	
	std::unique_ptr<Statements> suite();

	std::unique_ptr<IfStatement> if_stmt();	
	std::unique_ptr<ForStatement> for_stmt();

	std::shared_ptr<Function> func_def();
	std::unique_ptr<Statement> call();
	std::vector<std::string> parameter_list();

	std::unique_ptr<ExprNode> subscription();
	std::unique_ptr<ExprNode> array_init();
	std::unique_ptr<ArrayOps> array_ops();
	std::unique_ptr<ExprNode> array_len();

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
    void die(std::string const &where, std::string const &message,
			 Token &token);
};

#endif
