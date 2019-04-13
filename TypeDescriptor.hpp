/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include <iostream>
#include <string>
#include <memory>

#include "Token.hpp"

class TypeDescriptor {
public:
	enum types { INTEGER, DOUBLE, BOOLEAN, STRING };

	explicit TypeDescriptor(types type);
	virtual ~TypeDescriptor() = default;
	
	types &type();

private:
	types _type;	
};

class NumberDescriptor : public TypeDescriptor {
public:
	explicit NumberDescriptor(types descType);
	union {
		int intValue;
		double doubleValue;
		bool boolValue;
	} value;	
};

class StringDescriptor : public TypeDescriptor {
public:
	explicit StringDescriptor(types descType);
	std::string value;
};

void printValue (TypeDescriptor *desc);

bool evaluateBool(TypeDescriptor *desc);

void changeSign(TypeDescriptor *desc);

std::shared_ptr<TypeDescriptor> negate(TypeDescriptor *desc);

std::shared_ptr<TypeDescriptor> stringOperations(StringDescriptor *lValue,
												 StringDescriptor *rValue,
												 Token const &tok);

std::shared_ptr<TypeDescriptor> relOperations(NumberDescriptor *lDesc,
											  NumberDescriptor *rDesc,
											  Token const &tok);

std::shared_ptr<TypeDescriptor> arithOperations(NumberDescriptor *lDesc,
												NumberDescriptor *rDesc,
												Token const &tok);

#endif //EXPRINTER_TYPEDESCRIPTOR_HPP	
