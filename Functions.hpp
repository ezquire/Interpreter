/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_FUNCTIONS_HPP
#define EXPRINTER_FUNCTIONS_HPP

#include "Statements.hpp"

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
