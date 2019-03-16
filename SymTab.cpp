//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1

void SymTab::setValueFor(std::string vName, int value) {
    // Define a variable by setting its initial value.
#ifdef DEBUG
    std::cout << vName << " <- " << value << std::endl;
#endif
    symTab[vName] = value;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

int SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
#ifdef DEBUG
    std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;
#endif
    return symTab.find(vName)->second;
}
