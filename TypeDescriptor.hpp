/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include "Token.hpp"

#include <vector>

class TypeDescriptor {
public:
	enum types { INTEGER, DOUBLE, BOOLEAN, STRING, NULLARRAY, STRINGARRAY,
				 NUMBERARRAY };
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

class StringArray : public TypeDescriptor {
public:
	explicit StringArray(types descType);
	int sArraySize() { return stringArray.size(); }
	void sPop() { stringArray.pop_back(); }
	void sAppend(std::string element) { stringArray.push_back(element); }	
	std::vector<std::string> stringArray;
};

class NumberArray : public TypeDescriptor {
public:
	explicit NumberArray(types descType);
	int nArraySize() { return numberArray.size(); }
	void nPop() { numberArray.pop_back(); }
	void nAppend(int element) { numberArray.push_back(element); }
	std::vector<int> numberArray;
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
