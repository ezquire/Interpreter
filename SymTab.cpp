/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include <iostream>
#include <memory>
#include "SymTab.hpp"
#include "TypeDescriptor.hpp"
#include "Token.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1

void SymTab::setValueFor(std::string const &vName,
						 std::shared_ptr<TypeDescriptor> rDesc) {
    // Define a variable by setting its initial value.
	symTab[vName] = std::move(rDesc);
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
    return symTab.find(vName) != symTab.end();
}

std::shared_ptr<TypeDescriptor> SymTab::getValueFor(std::string const &vName) {
    if( !isDefined(vName) ) {
        std::cout << "SymTab::getValueFor: " << vName;
		std::cout << " has not been defined.\n";
        exit(1);
    }
#ifdef DEBUG
    std::cout << "SymTab::getValueFor: " << vName << " contains ";
	printValue( symTab.find(vName)->second.get() );
	std::cout << std::endl;
#endif
	
    return symTab.find(vName)->second;
}


