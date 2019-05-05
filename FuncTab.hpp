/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_FUNCTAB_HPP
#define EXPRINTER_FUNCTAB_HPP

#include "Statements.hpp"

class Function;

class FuncTab {
public:
	FuncTab() = default;
	void addFunction(std::string const &id, std::shared_ptr<Function> func);
	bool isDefined(std::string const &id);
	std::shared_ptr<Function> getFunction(std::string const &id);
	void print();
private:
	std::map<std::string, std::shared_ptr<Function>> _functions;
};

#endif
