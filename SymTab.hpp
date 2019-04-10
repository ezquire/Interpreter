//
// Created by Ali A. Kooshesh on 2/5/19.
//create t

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
//#include "TypeDescriptor.hpp"
// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.


struct TypeDescriptor {

    enum types { INTEGER, DOUBLE, BOOL, STRING, CHAR};

    TypeDescriptor(types type): _type{type} {}

    types & type() {return _type; }

    virtual ~TypeDescriptor() {}

  private:
    types _type;
  };

struct NumberDescriptor: public TypeDescriptor {
    // Number descriptor includes integer, double, and boolean datatypes.                                                                                                                                   
    NumberDescriptor (types descType): TypeDescriptor(descType) {}
    union {
      int intValue;
      double doubleValue;
      short boolValue;
    } value;


  };

struct StringDescriptor: public TypeDescriptor {

  // string descriptor includes string and char datatypes.
  StringDescriptor (types descType): TypeDescriptor(descType) {}

  //  char charValue;
  std::string strValue;


    };

class SymTab {
public:
 
    void printValue(TypeDescriptor * desc);
    void printValue(std::string vName);
    void setValueFor(std::string vName, int value);
    void setValueFor(std::string vName, std::string str);
    bool isDefined(std::string vName);
    TypeDescriptor *getValueFor(std::string vName);


private:
    std::map<std::string, TypeDescriptor *> symTab;
};


#endif //EXPRINTER_SYMTAB_HPP
