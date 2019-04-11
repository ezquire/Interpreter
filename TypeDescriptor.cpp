/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include <memory>

#include "TypeDescriptor.hpp"
#include "Token.hpp"

// Type Descriptor
TypeDescriptor::TypeDescriptor(types type): _type{type} {}

TypeDescriptor::types &TypeDescriptor::type() { return _type; }

// Number Descriptor
NumberDescriptor::NumberDescriptor(types descType): TypeDescriptor(descType) {}

// String Descriptor
StringDescriptor::StringDescriptor(types descType): TypeDescriptor(descType) {}

// Print function
void printValue(TypeDescriptor *desc) {	
	auto sDesc = dynamic_cast<StringDescriptor *>(desc);
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
	// dynamic_cast will return a nullptr if
	// desc is not of datatype NumberDescritpr.
	if( nDesc != nullptr ) { // We have a number
		// desc must have been of type NumberDescriptor
		if( nDesc->type() == TypeDescriptor::INTEGER )
			std::cout << nDesc->value.intValue;
		else if( nDesc->type() == TypeDescriptor::DOUBLE )
			std::cout << nDesc->value.doubleValue;
		else if( nDesc->type() == TypeDescriptor::BOOLEAN )
			std::cout << nDesc->value.boolValue;
		else std::cout << "Misconfigured union type." << std::endl;		
	} else if( sDesc != nullptr ) // we must have a string
		std::cout << sDesc->value;
	else { // we don't know the type
		std::cout << "Unable to cast TypeDescriptor to a known type\n";
		exit(1);
	}
}

// Bool Function
bool evaluateBool(TypeDescriptor *desc) {
	NumberDescriptor *numDesc = dynamic_cast<NumberDescriptor *>(desc);
	if(numDesc == nullptr) {
		std::cout << "Error: invalid cast to non-number type\n";
		exit(1);
	}
	auto type = numDesc->type();
	if(type == TypeDescriptor::BOOLEAN) {
		return numDesc->value.boolValue;
	} else if(type == TypeDescriptor::DOUBLE) {
		return numDesc->value.doubleValue != 0;
	} else
		return numDesc->value.intValue != 0;
}

// Unary Operations
void changeSign(TypeDescriptor *desc) {
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
	if(nDesc == nullptr) {
		std::cout << "changeSign() invalid cast\n";
		exit(1);
	} else if (nDesc->type() == TypeDescriptor::INTEGER)
		nDesc->value.intValue *= -1;
	else if (nDesc->type() == TypeDescriptor::DOUBLE)
		nDesc->value.doubleValue *= -1;
}

std::unique_ptr<TypeDescriptor> negate(TypeDescriptor *desc) {
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
	if(nDesc == nullptr) {
		std::cout << "negate() invalid cast\n";
		exit(1);
	} else {
		std::unique_ptr<NumberDescriptor> ret =
			std::make_unique<NumberDescriptor>(TypeDescriptor::BOOLEAN);
		if(nDesc->type() == TypeDescriptor::INTEGER) {
			ret->value.boolValue = nDesc->value.intValue == 0 ? true:false;
			return ret;
		} else if(nDesc->type() == TypeDescriptor::DOUBLE) {
			ret->value.boolValue = nDesc->value.doubleValue == 0 ? true:false;
			return ret;
		} else {
			ret->value.boolValue = nDesc->value.boolValue == 0 ? true:false;
			return ret;
		}
	}
}

// String Operations
std::unique_ptr<TypeDescriptor> stringOperations(StringDescriptor *lValue,
												 StringDescriptor *rValue,
												 Token tok) {
	if(tok.isAdditionOperator()) { // We have concatenation
		std::unique_ptr<StringDescriptor> ret =
			std::make_unique<StringDescriptor>(TypeDescriptor::STRING);
		ret->value = lValue->value + rValue->value;
		return ret;
	} else { // We have a boolean operator
		std::unique_ptr<NumberDescriptor> ret =
			std::make_unique<NumberDescriptor>(TypeDescriptor::BOOLEAN);
		if (tok.isEqual()) {
			ret->value.boolValue =
				lValue->value == rValue->value ? true:false;
			return ret;
		} else if (tok.isNotEqual()) {
			ret->value.boolValue =
				lValue->value != rValue->value ? true:false;
			return ret;
		} else if ( tok.isGreaterThan() ) {
			ret->value.boolValue =
				lValue->value > rValue->value ? true:false;
			return ret;
		} else if ( tok.isGreaterThanEqual() ) {
			ret->value.boolValue =
				lValue->value >= rValue->value ? true:false;
			return ret;
		} else if ( tok.isLessThan() ) {
			ret->value.boolValue =
				lValue->value < rValue->value ? true:false;
			return ret;
		} else if ( tok.isLessThanEqual() ) {
			ret->value.boolValue =
				lValue->value <= rValue->value ? true:false;
			return ret;
		} else {
			std::cout << "InfixExprNode::evaluate: don't know how to ";
			std:: cout << "evaluate this operator with strings\n";
			tok.print();
			std::cout << std::endl;
			exit(2);
		}
	}
}

std::unique_ptr<TypeDescriptor> numberOperations (NumberDescriptor *lValue,
												  NumberDescriptor *rValue,
												  Token tok) {
	;
}
