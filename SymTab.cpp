//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
#include "TypeDescriptor.hpp"
#include "Token.hpp"

// Uncomment the line below to enable debugging
//#define DEBUG 1

void SymTab::setValueFor(std::string vName, Token tok) {
    // Define a variable by setting its initial value.
	if( tok.isWholeNumber() ) {
		NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
		desc->value.intValue = tok.getWholeNumber();
		symTab[vName] = desc;
#ifdef DEBUG
		std::cout<< "\nHit SymTab::setValueFor tok.isWholeNumber()\n";
		std::cout << vName << " <- ";
		printValue(desc);
		std::cout << std::endl;
#endif
	}
	else if( tok.isFloat() ) {
		NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::DOUBLE);
		desc->value.doubleValue = tok.getFloat();
		symTab[vName] = desc;
#ifdef DEBUG
		std::cout << "\nHit SymTab::setValueFor tok.isFloat()\n";
		std::cout << vName << " <- ";
		printValue(desc);
		std::cout << std::endl;
#endif
	}
	else if( tok.isName() ) {
#ifdef DEBUG
		std::cout << "\nHit SymTab::setValueFor tok.isName()\n";
		std::cout << "vName: " << vName << std::endl;
#endif
		symTab[vName] = getValueFor( tok.getName() );
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
