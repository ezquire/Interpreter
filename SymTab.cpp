//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"

void SymTab::printValue(TypeDescriptor * desc) {
  // If desc is of datatype NumberDescriptor, this function
  // print the value that is store in it. Otherwise,
  // it just returns.

  
  // Use dynamic_cast to down-cast, from TypeDescriptor to
  // NumberDescriptr.
  NumberDescriptor * nDesc = dynamic_cast< NumberDescriptor * >(desc);
  // dynamic_cast will return a nullptr if
  // desc is not of datatype NumberDescriptr.
  if( nDesc == nullptr )
    return;

  // desc must have been of type NumberDescriptr
  if( nDesc->type() == TypeDescriptor::INTEGER )
    std::cout<< nDesc->value.intValue<< std::endl;
  else if( nDesc->type() == TypeDescriptor::DOUBLE )
    std::cout<< nDesc->value.doubleValue << std::endl;
  else if( nDesc->type() == TypeDescriptor::BOOL )
    std::cout << nDesc->value.boolValue << std::endl;
  else
    std::cout<< "Misconfigured union type. " << std :: endl;
  
}


void SymTab::printValue(std::string vName) {

  if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }

    if(symTab.find(vName)->second->type() == TypeDescriptor::INTEGER) {

       NumberDescriptor * nDesc = dynamic_cast<NumberDescriptor *>(symTab.find(vName)->second);
       std::cout << nDesc->value.intValue;
      

    }

    else {

        StringDescriptor * nDesc = dynamic_cast<StringDescriptor *>(symTab.find(vName)->second);
        std::cout<< nDesc->strValue;
     
    }
}

void SymTab::setValueFor(std::string vName, int value) {
    // Define a variable by setting its initial value.
    std::cout << vName << " <- " << value << std::endl;
    //symTab[vName] = value;
    NumberDescriptor * desc = new NumberDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue = value;
    desc->value.doubleValue = double(value);
    desc->value.boolValue = short(value);
    
    symTab[vName] = desc;
}

void SymTab::setValueFor(std::string vName, std::string str){

  std::cout << vName << " <- " << str << std::endl;

  StringDescriptor * desc = new StringDescriptor(TypeDescriptor::STRING);
  desc->strValue = str;

  symTab[vName] = desc;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}


TypeDescriptor * SymTab::getValueFor(std::string vName) {

      if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
      }
      
      /*
      if(symTab.find(vName)->second->type() == TypeDescriptor::INTEGER) {

        NumberDescriptor * nDesc = dynamic_cast<NumberDescriptor *>(symTab.find(vName)->second);
        std::cout << "SymTab::getValueFor: " << vName << " contains " << nDesc->value.intValue << std::endl;
   
      }

     else if(symTab.find(vName)->second->type() == TypeDescriptor::STRING){

         StringDescriptor * nDesc = dynamic_cast<StringDescriptor *>(symTab.find(vName)->second);
         std::cout << "SymTab::getValueFor: " << vName << " contains " << nDesc->strValue << std::endl;
      
       }
      */
 
      return symTab.find(vName)->second;
      
}
    
   
