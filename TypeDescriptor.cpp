/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "TypeDescriptor.hpp"

// Type Descriptor
TypeDescriptor::TypeDescriptor(types type): _type{type} {}

TypeDescriptor::types &TypeDescriptor::type() { return _type; }


// Number Descriptor
NumberDescriptor::NumberDescriptor(types descType): TypeDescriptor(descType) {}


// String Descriptor
StringDescriptor::StringDescriptor(types descType): TypeDescriptor(descType) {}

// Print function
void printValue(TypeDescriptor *desc) {	
	StringDescriptor *sDesc;
	NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(desc);
	// dynamic_cast will return a nullptr if
	// desc is not of datatype NumberDescritpr.
	if( nDesc == nullptr ) { // We don't have a number so check for string
		sDesc = dynamic_cast<StringDescriptor *>(desc);
		if( sDesc == nullptr ) {
			std::cout << "Unable to cast to valid type\n";
			exit(1);
		}
	}
	// desc must have been of type NumberDescriptor or StringDescriptor
	if( nDesc->type() == TypeDescriptor::INTEGER )
		std::cout << nDesc->value.intValue;
	else if( nDesc->type() == TypeDescriptor::DOUBLE )
		std::cout << nDesc->value.doubleValue;
	else if( nDesc->type() == TypeDescriptor::BOOLEAN )
		std::cout << nDesc->value.boolValue;
	else if( sDesc->type() == TypeDescriptor::STRING )
		std::cout << sDesc->value;
	else std::cout << "Misconfigured union type." << std::endl;
}

// Type Checking
bool isValidType(TypeDescriptor *lDesc, TypeDescriptor *rDesc) {
	auto lType = lDesc->type();
	auto rType = rDesc->type();

	if ( lType == rType )
		return true;
	else if ( lType == TypeDescriptor::BOOLEAN
			  && ( rType == TypeDescriptor::DOUBLE
				   || rType == TypeDescriptor::INTEGER ))
		return true;
	else if ( rType == TypeDescriptor::BOOLEAN
			  && ( lType == TypeDescriptor::DOUBLE
				   || lType == TypeDescriptor::INTEGER ))
		return true;
	else if ( (rType == TypeDescriptor::INTEGER
			   && lType == TypeDescriptor::DOUBLE)
			  || (lType == TypeDescriptor::INTEGER
				  && rType == TypeDescriptor::DOUBLE) )
		return true;
	else return false;
}
