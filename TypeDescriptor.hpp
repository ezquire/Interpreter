/*
 * Created by Tyler Gearing on 3/14/2019
 *
 */

#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include <iostream>
#include <string>

class TypeDescriptor {
public:
	enum types { INTEGER, DOUBLE, BOOLEAN, STRING };

	TypeDescriptor(types type);
	
	types &type();

	virtual ~TypeDescriptor() {}

private:
	types _type;	
};

class NumberDescriptor : public TypeDescriptor {
public:
	NumberDescriptor(types descType);

	union {
		int intValue;
		double doubleValue;
		bool boolValue;
	} value;	
};

class StringDescriptor : public TypeDescriptor {
public:
	StringDescriptor(types descType);

	std::string value;
};

void printValue (TypeDescriptor *desc);

bool isValidType(TypeDescriptor *desc1, TypeDescriptor *desc2);

#endif //EXPRINTER_TYPEDESCRIPTOR_HPP	
