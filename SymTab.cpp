//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
#include "TypeDescriptor.hpp"


void SymTab::setValueFor(std::string vName, int value) {
    // Define a variable by setting its initial value.
    std::cout << vName << " <- " << value << std::endl;
    //symTab[vName] = value; Need it for Number and for string so 2 setValues...
    NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue =  value;
    symTab[vName] = desc;

}
void SymTab::setValueFor(std::string vName, std::string value) {
    // Define a variable by setting its initial value.
    std::cout << vName << " <- " << value << std::endl;
    //symTab[vName] = value;
    StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING, value);
    symTab[vName] = desc;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}


TypeDescriptor* SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    TypeDescriptor* TokDesc = symTab.find(vName)->second;
    NumberDescriptor* Toknum = dynamic_cast<NumberDescriptor*>( TokDesc );
    if (Toknum == nullptr) {
        // This wasn't an integer variable, only other option currently is a string
        StringDescriptor* Tokstr = dynamic_cast<StringDescriptor*>( TokDesc );
        std::string StrValue = Tokstr->strVal();
        std::cout << "SymTab::getValueFor: " << vName << " contains " << StrValue << std::endl;

    }
    else // The variable was an integer
    {
        int IntValue = Toknum->value.intValue;
        std::cout << "SymTab::getValueFor: " << vName << " contains " << IntValue << std::endl;
    }

    return symTab.find(vName)->second;


}
/*
int SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;
    //return symTab.find(vName)->second;
    return symTab.find(vName)->second;
}

 */

