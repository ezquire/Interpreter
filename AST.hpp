/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_AST_HPP
#define EXPRINTER_AST_HPP

#include "Statements.hpp"
#include "FuncTab.hpp"

class AST {
public:
	AST();
	AST(std::unique_ptr<Statements> stmts, std::unique_ptr<FuncTab> funcTab);
	std::unique_ptr<Statements> stmts();
	std::unique_ptr<FuncTab> funcTab();
private:
	std::unique_ptr<Statements> _stmts;
	std::unique_ptr<FuncTab> _funcTab;
};

#endif //EXPRINTER_AST_HPP
