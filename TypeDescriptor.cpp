/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include <memory>
#include <cmath>

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
		std::cout << "evaluateBool() invalid cast.\n";
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

std::shared_ptr<TypeDescriptor> negate(TypeDescriptor *desc) {
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
	if(nDesc == nullptr) {
		std::cout << "negate() invalid cast\n";
		exit(1);
	} else {
		std::shared_ptr<NumberDescriptor> ret =
			std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);
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
std::shared_ptr<TypeDescriptor> stringOperations(StringDescriptor *lValue,
												 StringDescriptor *rValue,
												 Token tok) {	
	if(tok.isAdditionOperator()) { // We have concatenation
		std::shared_ptr<StringDescriptor> ret =
			std::make_shared<StringDescriptor>(TypeDescriptor::STRING);
		ret->value = lValue->value + rValue->value;
		return ret;
	} else { // We have a boolean operator
		std::shared_ptr<NumberDescriptor> ret =
			std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);
		if (tok.isEqual()) {
			ret->value.boolValue =
				lValue->value == rValue->value ? true:false;
			return ret;
		} else if (tok.isNotEqual()) {
			ret->value.boolValue =
				lValue->value != rValue->value ? true:false;
			return ret;
		} else if (tok.isGreaterThan()) {
			ret->value.boolValue =
				lValue->value > rValue->value ? true:false;
			return ret;
		} else if (tok.isGreaterThanEqual()) {
			ret->value.boolValue =
				lValue->value >= rValue->value ? true:false;
			return ret;
		} else if (tok.isLessThan()) {
			ret->value.boolValue =
				lValue->value < rValue->value ? true:false;
			return ret;
		} else if (tok.isLessThanEqual()) {
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

std::shared_ptr<TypeDescriptor> relOperations(NumberDescriptor *lDesc,
											  NumberDescriptor *rDesc,
											  Token tok) {	
	std::shared_ptr<NumberDescriptor> ret =
		std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);

	auto lType = lDesc->type();
	auto rType = rDesc->type();

	if(lType == TypeDescriptor::DOUBLE || rType == TypeDescriptor::DOUBLE) {
		std::cout << "Double type not supported yet\n";
		exit(1);
	}

	if( tok.isEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue == rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue == rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue == rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue == rDesc->value.intValue ? true:false;
			return ret;
		}
	} else if( tok.isNotEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue != rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue != rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue != rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue != rDesc->value.intValue ? true:false;
			return ret;
		}
	} else if( tok.isLessThan() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue < rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue < rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue < rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue < rDesc->value.intValue ? true:false;
			return ret;
		}
	} else if( tok.isLessThanEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue <= rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue <= rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue <= rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue <= rDesc->value.intValue ? true:false;
			return ret;
		}
	} else if( tok.isGreaterThan() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue > rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue > rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue > rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue > rDesc->value.intValue ? true:false;
			return ret;
		}
	} else if( tok.isGreaterThanEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue >= rDesc->value.intValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue >= rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue >= rDesc->value.boolValue ? true:false;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue >= rDesc->value.intValue ? true:false;
			return ret;
		}
	} else {
		std::cout << "relOperations() not a valid operator";
		exit(1);
	}
}

std::shared_ptr<TypeDescriptor> arithOperations(NumberDescriptor *lDesc,
												NumberDescriptor *rDesc,
												Token tok) {
	std::shared_ptr<NumberDescriptor> ret =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);

	auto lType = lDesc->type();
	auto rType = rDesc->type();

	if(lType == TypeDescriptor::DOUBLE || rType == TypeDescriptor::DOUBLE) {
		std::cout << "Double type not supported yet\n";
		exit(1);
	}

	if(tok.isAdditionOperator()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.intValue =
				lDesc->value.intValue + rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue = 
				lDesc->value.intValue + rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue = 
				lDesc->value.boolValue + rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.intValue = 
				lDesc->value.boolValue + rDesc->value.intValue;
			return ret;
		}
	} else if (tok.isSubtractionOperator()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.intValue =
				lDesc->value.intValue - rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue = 
				lDesc->value.intValue - rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue = 
				lDesc->value.boolValue - rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.intValue = 
				lDesc->value.boolValue - rDesc->value.intValue;
			return ret;
		}
	} else if (tok.isMultiplicationOperator()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.intValue =
				lDesc->value.intValue * rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue = 
				lDesc->value.intValue * rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.intValue =
				lDesc->value.boolValue * rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.intValue = 
				lDesc->value.boolValue * rDesc->value.intValue;
			return ret;
		}
	} else if (tok.isDivisionOperator()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					lDesc->value.intValue / rDesc->value.intValue;
				return ret;
			}
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					lDesc->value.intValue / rDesc->value.boolValue;
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					lDesc->value.boolValue / rDesc->value.boolValue;
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					lDesc->value.boolValue / rDesc->value.intValue;
				return ret;
			}
		}
	} else if (tok.isFloorDivision()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.intValue / rDesc->value.intValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.intValue / rDesc->value.boolValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.boolValue / rDesc->value.boolValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: division by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.boolValue / rDesc->value.intValue);
				return ret;
			}
		}
	} else if (tok.isModuloOperator()) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: modulo by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.intValue % rDesc->value.intValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: modulo by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.intValue % rDesc->value.boolValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			if(rDesc->value.boolValue == 0) {
				std::cout << "Error: modulo by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.boolValue % rDesc->value.boolValue);
				return ret;
			}
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			if(rDesc->value.intValue == 0) {
				std::cout << "Error: modulo by zero\n";
				exit(1);
			} else {
				ret->value.intValue =
					floor(lDesc->value.boolValue % rDesc->value.intValue);
				return ret;
			}
		}
	} else {
		std::cout << "arithOperations() not a valid operator";
		exit(1);
	}
}
