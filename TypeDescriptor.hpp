//
// Created by Johnathan Soto on 2019-03-25.
//

#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include <string>
#include <map>

struct TypeDescriptor {
    // The base class for datatype descriptors.

    // This enum can expand to include additional types.
    enum types {INTEGER, STRING, BOOL};

    TypeDescriptor(types type);

    types &type();

    virtual ~TypeDescriptor() {}

private:
    types _type;
};

struct NumberDescriptor: public TypeDescriptor {
    // Number descriptor includes integer datatypes.

    NumberDescriptor(types descType);

    union {
        int intValue;
        bool boolValue;
    } value;

};

struct StringDescriptor: public TypeDescriptor {
    // String descriptor includes string datatypes.

    StringDescriptor(types descType, std::string val);
    std::string strVal();

private:
    std::string strValue;

};

#endif //EXPRINTER_TYPEDESCRIPTOR_HPP
