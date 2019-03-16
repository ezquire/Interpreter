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


// Print function
void printValue(TypeDescriptor *desc) {
	
	NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(desc);

	// dynamic_cast will return a nullptr if
	// desc is not of datatype NumberDescritpr.
	if( nDesc == nullptr )
		return;

	// desc must have been of type NumberDescriptr
	if( nDesc->type() == TypeDescriptor::INTEGER )std::cout << nDesc->value.intValue << std::endl;
	else if( nDesc->type() == TypeDescriptor::DOUBLE )std::cout << nDesc->value.doubleValue << std::endl;
	else if( nDesc->type() == TypeDescriptor::BOOLEAN )std::cout << nDesc->value.boolValue << std::endl;
	else std::cout << "Misconfigured union type." << std::endl;
}
											
