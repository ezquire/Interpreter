/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Functions.hpp"

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
