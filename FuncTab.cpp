/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "FuncTab.hpp"

// FuncTab
void FuncTab::addFunction(std::string const &id,
							std::shared_ptr<Function> func) {
	_functions[id] = func;
}

bool FuncTab::isDefined(std::string const &id) {
	return _functions.find(id) != _functions.end();
}

std::shared_ptr<Function> FuncTab::getFunction(std::string const &id) {
    if( !isDefined(id) ) {
        std::cout << "FuncTab::getFunction: " << id;
		std::cout << " has not been defined.\n";
        exit(1);
    }
    return _functions.find(id)->second;
}

void FuncTab::print() {
	for(auto &f: _functions) {
		std::cout << f.first << ": ";
		f.second->print();
		std::cout << std::endl;
	}
}
