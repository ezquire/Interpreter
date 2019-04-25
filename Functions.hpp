/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_FUNCTIONS_HPP
#define EXPRINTER_FUNCTIONS_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "SymTab.hpp"
#include "Statements.hpp"

class Function {
public:
	Function();
	Function(std::string id, std::vector<std::string> params,
			 std::unique_ptr<Statements> suite);
	void evaluate(SymTab &symTab);
	void print();
	std::string &id();
	std::vector<std::string> &params();
	std::unique_ptr<Statements> &suite();
private:
	std::string _id;
	std::vector<std::string> _parameters;
	std::unique_ptr<Statements> _suite;
};

class Functions {
public:
	Functions() = default;
	void addFunction(std::string const &id, std::shared_ptr<Function> func);
	bool isDefined(std::string const &id);
	std::shared_ptr<Function> getFunction(std::string const &id);
private:
	std::map<std::string, std::shared_ptr<Function>> _functions;
};

#endif
