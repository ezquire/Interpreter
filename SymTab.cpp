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
