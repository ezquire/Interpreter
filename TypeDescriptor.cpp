/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "TypeDescriptor.hpp"

#include <cmath>

// Type Descriptor
TypeDescriptor::TypeDescriptor(types type): _type{type} {}

TypeDescriptor::types &TypeDescriptor::type() { return _type; }

// Number Descriptor
NumberDescriptor::NumberDescriptor(types descType): TypeDescriptor(descType) {}

// String Descriptor
StringDescriptor::StringDescriptor(types descType): TypeDescriptor(descType) {}

// String Array Descriptor
StringArray::StringArray(types descType): TypeDescriptor(descType) {}

std::string StringArray::sSub(int n){
    if( n > (int)stringArray.size()-1 || n < 0 ) {
        std::cout << "StringArray::sSub array index out of bounds\n";
        exit(1);
    }
	return stringArray[n];
}

void StringArray::sPopIndex(TypeDescriptor *desc) {
	auto index = dynamic_cast<NumberDescriptor*>(desc);
	if(index == nullptr || desc->type() != INTEGER) {
		std::cout << "StringArray::sPop error, index must be an integer\n";
		exit(1);
	} else if(index->value.intValue < 0 ||
			  index->value.intValue > (int)stringArray.size()-1) {
		std::cout << "StringArray::sPopIndex error, array index out of";
		std::cout << " bounds\n";
		exit(1);
	} else
		stringArray.erase(stringArray.begin() + index->value.intValue);
}

// Number Array descriptor
NumberArray::NumberArray(types descType): TypeDescriptor(descType) {}

int NumberArray::nSub(int n){
    if( n > (int)numberArray.size()-1 || n < 0 ){
        std::cout << "StringArray::nSub array index out of bounds\n";
        exit(1);
    }
	return numberArray[n];
}

void NumberArray::nPopIndex(TypeDescriptor *desc) {
	auto index = dynamic_cast<NumberDescriptor*>(desc);
	if(index == nullptr || desc->type() != INTEGER) {
		std::cout << "NumberArray::nPop error, index must be an integer\n";
		exit(1);
	} else if(index->value.intValue < 0 ||
			  index->value.intValue > (int)numberArray.size()-1) {
		std::cout << "NumberArray::nPopIndex error, array index out of";
		std::cout << " bounds\n";
		exit(1);
	} else
		numberArray.erase(numberArray.begin() + index->value.intValue);
}

// Print function
void printValue(TypeDescriptor *desc) {	
	auto sDesc = dynamic_cast<StringDescriptor *>(desc);
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
    auto sArray = dynamic_cast<StringArray *>(desc);
    auto nArray = dynamic_cast<NumberArray *>(desc);
	// dynamic_cast will return a nullptr if
	// desc is not that type.
	if( nDesc != nullptr ) { // We have a number
		// desc must have been of type NumberDescriptor
		if( nDesc->type() == TypeDescriptor::INTEGER )
			std::cout << nDesc->value.intValue;
		else if( nDesc->type() == TypeDescriptor::DOUBLE )
			std::cout << nDesc->value.doubleValue;
		else if( nDesc->type() == TypeDescriptor::BOOLEAN )
			std::cout << nDesc->value.boolValue;
		else std::cout << "Misconfigured union type." << std::endl;		
	}
	else if( desc->type() == TypeDescriptor::NULLARRAY)
		std::cout << "[]" << std::endl;
	else if( sDesc != nullptr )
		std::cout << sDesc->value;
    else if(sArray != nullptr) {
		std::cout << '[';
		for(unsigned i = 0; i < sArray->stringArray.size()-1; ++i)
			std::cout << '\'' << sArray->stringArray[i] << '\'' << ", ";
		std::cout << '\'';
		std::cout << sArray->stringArray[sArray->stringArray.size()-1] << "\'";
		std::cout << ']';
	} else if(nArray != nullptr) {
		std::cout << '[';
		for(unsigned i = 0; i < nArray->numberArray.size()-1; ++i)
			std::cout << nArray->numberArray[i] << ", ";
		std::cout << nArray->numberArray[nArray->numberArray.size()-1] << ']';
	} else { // we don't know the type
		std::cout << "None";
	}
}

// Bool Function
bool evaluateBool(TypeDescriptor *desc) {
	auto numDesc = dynamic_cast<NumberDescriptor *>(desc);
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

// Negate
std::shared_ptr<TypeDescriptor> negate(TypeDescriptor *desc) {
	auto nDesc = dynamic_cast<NumberDescriptor *>(desc);
	if(nDesc == nullptr) {
		std::cout << "negate() invalid cast\n";
		exit(1);
	} else {
		std::shared_ptr<NumberDescriptor> ret =
			std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);
		if(nDesc->type() == TypeDescriptor::INTEGER) {
			ret->value.boolValue = nDesc->value.intValue == 0;
			return ret;
		} else if(nDesc->type() == TypeDescriptor::DOUBLE) {
			ret->value.boolValue = nDesc->value.doubleValue == 0;
			return ret;
		} else {
			ret->value.boolValue = nDesc->value.boolValue == 0;
			return ret;
		}
	}
}

// String Operations
std::shared_ptr<TypeDescriptor> stringOperations(StringDescriptor *lValue,
												 StringDescriptor *rValue,
												 Token const &tok) {
	if(tok.isAdditionOperator()) { // We have concatenation
		std::shared_ptr<StringDescriptor> ret =
			std::make_shared<StringDescriptor>(TypeDescriptor::STRING);
		ret->value = lValue->value + rValue->value;
		return ret;
	} else { // We have a boolean operator
		std::shared_ptr<NumberDescriptor> ret =
			std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);
		if (tok.isEqual()) {
			ret->value.boolValue = lValue->value == rValue->value;
			return ret;
		} else if (tok.isNotEqual()) {
			ret->value.boolValue = lValue->value != rValue->value;
			return ret;
		} else if (tok.isGreaterThan()) {
			ret->value.boolValue = lValue->value > rValue->value;
			return ret;
		} else if (tok.isGreaterThanEqual()) {
			ret->value.boolValue = lValue->value >= rValue->value;
			return ret;
		} else if (tok.isLessThan()) {
			ret->value.boolValue = lValue->value < rValue->value;
			return ret;
		} else if (tok.isLessThanEqual()) {
			ret->value.boolValue = lValue->value <= rValue->value;
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

// Relational Operations
std::shared_ptr<TypeDescriptor> relOperations(NumberDescriptor *lDesc,
											  NumberDescriptor *rDesc,
											  Token const &tok) {
	std::shared_ptr<NumberDescriptor> ret =
		std::make_shared<NumberDescriptor>(TypeDescriptor::BOOLEAN);

	auto lType = lDesc->type();
	auto rType = rDesc->type();

	if(lType == TypeDescriptor::DOUBLE || rType == TypeDescriptor::DOUBLE) {
		std::cout << "Double type not supported yet\n";
		exit(1);
	}

	else if( tok.isEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue == rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue == rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue == rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue == rDesc->value.intValue;
			return ret;
		}
	} else if( tok.isNotEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue != rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue != rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue != rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue != rDesc->value.intValue;
			return ret;
		}
	} else if( tok.isLessThan() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue < rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue < rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue < rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue < rDesc->value.intValue;
			return ret;
		}
	} else if( tok.isLessThanEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue <= rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue <= rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue <= rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue <= rDesc->value.intValue;
			return ret;
		}
	} else if( tok.isGreaterThan() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue > rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue > rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue > rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue > rDesc->value.intValue;
			return ret;
		}
	} else if( tok.isGreaterThanEqual() ) {
		if(lType == TypeDescriptor::INTEGER
		   && rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.intValue >= rDesc->value.intValue;
			return ret;
		} else if(lType == TypeDescriptor::INTEGER &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.intValue >= rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::BOOLEAN) {
			ret->value.boolValue = 
				lDesc->value.boolValue >= rDesc->value.boolValue;
			return ret;
		} else if(lType == TypeDescriptor::BOOLEAN &&
				  rType == TypeDescriptor::INTEGER) {
			ret->value.boolValue = 
				lDesc->value.boolValue >= rDesc->value.intValue;
			return ret;
		}
	} else {
		std::cout << "relOperations() not a valid operator";
		exit(1);
	}

	return nullptr;
}

// Arithmetic Operations
std::shared_ptr<TypeDescriptor> arithOperations(NumberDescriptor *lDesc,
												NumberDescriptor *rDesc,
												Token const &tok) {
	std::shared_ptr<NumberDescriptor> ret =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);

	auto lType = lDesc->type();
	auto rType = rDesc->type();

	if(lType == TypeDescriptor::DOUBLE || rType == TypeDescriptor::DOUBLE) {
		std::cout << "Double type not supported yet\n";
		exit(1);
	} else if(tok.isAdditionOperator()) {
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
	return nullptr;
}


