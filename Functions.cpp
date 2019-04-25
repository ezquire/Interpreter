/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Functions.hpp"
#include "SymTab.hpp"

// Functions
void Functions::addFunction(std::string const &id,
							std::shared_ptr<Function> func) {
	_functions[id] = func;
}

bool Functions::isDefined(std::string const &id) {
	return _functions.find(id) != _functions.end();
}

std::shared_ptr<Function> Functions::getFunction(std::string const &id) {
    if( !isDefined(id) ) {
        std::cout << "Functions::getFunction: " << id;
		std::cout << " has not been defined.\n";
        exit(1);
    }
    return _functions.find(id)->second;
}

// Function
Function::Function(): _id{""}, _suite{nullptr} {}

Function::Function(std::string id, std::vector<std::string> params,
				   std::unique_ptr<Statements> suite):
	_id{id}, _parameters{params}, _suite{std::move(suite)} {}

void Function::evaluate(SymTab &symTab) {
	std::cout << "Function::evaluate not implemented yet" << std::endl;
	exit(1);
}

std::string &Function::id() {
	return _id;
}

std::vector<std::string> &Function::params() {
	return _parameters;
}

std::unique_ptr<Statements> &Function::suite() {
	return _suite;
}
