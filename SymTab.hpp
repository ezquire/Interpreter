/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include "TypeDescriptor.hpp"
#include "Token.hpp"
#include <string>
#include <map>
#include <memory>

// This is a flat symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class SymTab {
public:
    //void setValueFor(std::string vName, Token tok);
	void setValueFor(std::string const &vName,
					 std::shared_ptr<TypeDescriptor>rDesc);
	void increment(std::string const &vName, int increment);
    bool isDefined(std::string const &vName);
	std::shared_ptr<TypeDescriptor> getValueFor(std::string const &vName);

private:
    std::map<std::string, std::shared_ptr<TypeDescriptor>> symTab;
};

#endif //EXPRINTER_SYMTAB_HPP
