/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "SymTab.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1
#define RETURN "retVal"

void SymTab::setValueFor(std::string const &vName,
						 std::shared_ptr<TypeDescriptor> rDesc) {
    // Define a variable by setting its initial value.
	if(scope.empty())
		global[vName] = std::move(rDesc);
	else
		scope.back()[vName] = std::move(rDesc);
}

void SymTab::increment(std::string const &vName, int increment) {
    // Define a variable by setting its initial value.
	auto rDesc = dynamic_cast<NumberDescriptor *>
		( getValueFor(vName).get() );
	if( rDesc == nullptr ) {
		std::cout << "SymTab::increment error casting TypeDescriptor\n";
		exit(1);
	}
	if( rDesc->type() != TypeDescriptor::INTEGER) {
		std::cout << "SymTab::increment only supports integers\n";
		exit(1);
	} else
	    rDesc->value.intValue += increment;
}

bool SymTab::isDefinedGlobal(std::string const &vName) {
	return global.find(vName) != global.end();
}

bool SymTab::isDefinedScope(std::string const &vName) {
	if(scope.empty())
		return false;
	return scope.back().find(vName) != scope.back().end();
}

std::shared_ptr<TypeDescriptor> SymTab::getValueFor(std::string const &vName) {
	if(scope.empty()) {
		if( !isDefinedGlobal(vName) ) {
			std::cout << "SymTab::getValueFor: " << vName;
			std::cout << " has not been defined.\n";
			exit(1);
		}
		return global.find(vName)->second;
	} else {
		if(isDefinedScope(vName))
			return scope.back().find(vName)->second;
		else if (isDefinedGlobal(vName))
			return global.find(vName)->second;
		else {
			std::cout << "SymTab::getValueFor: " << vName;
			std::cout << " has not been defined.\n";
			exit(1);
		}
	}
}

void SymTab::openScope(std::vector<std::string> params,
			   std::vector<std::shared_ptr<TypeDescriptor>> args) {
	std::map<std::string, std::shared_ptr<TypeDescriptor>> newScope;
	for(unsigned i = 0; i < params.size(); ++i)
		newScope[params[i]] = args[i];
	scope.push_back(newScope);
}

void SymTab::setReturnValue(std::shared_ptr<TypeDescriptor> ret) {
	global[RETURN] = ret;
}

std::shared_ptr<TypeDescriptor> SymTab::getReturnValue() {
	if( !isDefinedGlobal(RETURN) ) {
        std::cout << "SymTab::getReturnValue: returnVal";
		std::cout << " has not been defined.\n";
        exit(1);
    }
    return global.find(RETURN)->second;
}

void SymTab::closeScope() {
	scope.pop_back();
}

void SymTab::removeReturn() {
	global.erase(RETURN);
}


