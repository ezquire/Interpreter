/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "AST.hpp"

AST::AST(): _stmts{nullptr}, _funcTab{nullptr} {}

AST::AST(std::unique_ptr<Statements> stmts, std::unique_ptr<FuncTab> funcTab) :
	_stmts{std::move(stmts)}, _funcTab{std::move(funcTab)} {}

std::unique_ptr<Statements> AST::stmts() {
	return std::move(_stmts);
}

std::unique_ptr<FuncTab> AST::funcTab() {
	return std::move(_funcTab);
}
