/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "SymTab.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1

void SymTab::setValueFor(std::string const &vName,
						 std::shared_ptr<TypeDescriptor> rDesc) {
    // Define a variable by setting its initial value.
	if(scope.empty())
		global[vName] = std::move(rDesc);
	
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

bool SymTab::isDefined(std::string const &vName) {
    return global.find(vName) != global.end();
}

std::shared_ptr<TypeDescriptor> SymTab::getValueFor(std::string const &vName) {
    if( !isDefined(vName) ) {
        std::cout << "SymTab::getValueFor: " << vName;
		std::cout << " has not been defined.\n";
        exit(1);
    }
#ifdef DEBUG
    std::cout << "SymTab::getValueFor: " << vName << " contains ";
	printValue( global.find(vName)->second.get() );
	std::cout << std::endl;
#endif
	
    return global.find(vName)->second;
}

void SymTab::openScope(std::vector<std::string> params,
			   std::vector<std::shared_ptr<TypeDescriptor>> args) {
	std::cout << "SymTab::openScope need to implement" << std::endl;
	exit(1);
}

void SymTab::setReturnValue(std::shared_ptr<TypeDescriptor> ret) {
	global["returnVal"] = ret;
}

std::shared_ptr<TypeDescriptor> SymTab::getReturnValue() {
	if( !isDefined("returnVal") ) {
        std::cout << "SymTab::getReturnValue: returnVal";
		std::cout << " has not been defined.\n";
        exit(1);
    }
    return global.find("returnVal")->second;
}

void SymTab::closeScope() {
	scope.pop_back();
}
