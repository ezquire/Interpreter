//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
#include "TypeDescriptor.hpp"
#include "Token.hpp"

// Uncomment the line below to enable debugging
//#define DEBUG 1

void SymTab::setValueFor(std::string vName, TypeDescriptor *rDesc) {
    // Define a variable by setting its initial value.
	symTab[vName] = rDesc;
}

void SymTab::increment(std::string vName, int increment) {
    // Define a variable by setting its initial value.
	NumberDescriptor * rDesc = dynamic_cast<NumberDescriptor *>
		( getValueFor(vName) );
	if( rDesc == nullptr ) {
		std::cout << "SymTab::increment error casting TypeDescriptor\n";
		exit(1);
	}
	if( rDesc->type() != TypeDescriptor::INTEGER) {
		std::cout << "SymTab::increment only supports integers\n";
		exit(1);
	} else {
		rDesc->value.intValue += increment;
		symTab[vName] = rDesc;
	}
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor *SymTab::getValueFor(std::string vName) {
    if( !isDefined(vName) ) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
#ifdef DEBUG
    std::cout << "SymTab::getValueFor: " << vName << " contains ";
	printValue( symTab.find(vName)->second );
	std::cout << std::endl;
#endif
    return symTab.find(vName)->second;
}
