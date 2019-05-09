/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Expr.hpp"

// Uncomment the line below to enable debugging
// #define DEBUG 1
#define RETURN "retVal"

// ExprNode
ExprNode::ExprNode(Token token): _token{std::move(token)} {}

Token ExprNode::token() { return _token; }

// InfixExprNode
InfixExprNode::InfixExprNode(Token tk): ExprNode{std::move(tk)},
										_left(nullptr), _right(nullptr) {}

std::unique_ptr<ExprNode> &InfixExprNode::left() { return _left; }

std::unique_ptr<ExprNode> &InfixExprNode::right() { return _right; }

std::shared_ptr<TypeDescriptor> InfixExprNode::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
    // Evaluates an infix expression using a post-order
	// Traversal of the expression tree.
	auto lValue = left()->evaluate(symTab, funcTab);
	auto slDesc = dynamic_cast<StringDescriptor *>(lValue.get());
	auto lDesc = dynamic_cast<NumberDescriptor *>(lValue.get());
	if( right() == nullptr) { // we have some unary operator on left()
		if(token().isSubtractionOperator()) { // Negation operation
			changeSign(lValue.get());
			return lValue;
		} else if( token().isNotKeyword() ) { // Boolean Not operation
			return negate(lValue.get());
		} else {
				std::cout << "InfixExprNode::evaluate: don't know how to ";
				std:: cout << "evaluate this unary operator\n";
				token().print();
				std::cout << std::endl;
				exit(2);
		}
	} else if( token().isAndKeyword() && lDesc != nullptr) {
		auto lType = lDesc->type();
		if(lType == TypeDescriptor::BOOLEAN) {
			if(lDesc->value.boolValue == 0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		} else if(lType == TypeDescriptor::DOUBLE) {
			if(lDesc->value.doubleValue == 0.0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		} else if(lType == TypeDescriptor::INTEGER) {
			if(lDesc->value.intValue == 0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		}
	} else if( token().isOrKeyword() && lDesc != nullptr) {
		auto lType = lDesc->type();
		if(lType == TypeDescriptor::BOOLEAN) {
			if(lDesc->value.boolValue != 0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		} else if(lType == TypeDescriptor::DOUBLE) {
			if(lDesc->value.doubleValue != 0.0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		} else if(lType == TypeDescriptor::INTEGER) {
			if(lDesc->value.intValue != 0)
				return lValue;
			else return right()->evaluate(symTab, funcTab);;
		}
	} else { // We have a left() and a right()
		auto rValue = right()->evaluate(symTab, funcTab);
		auto srDesc = dynamic_cast<StringDescriptor *>(rValue.get());
		auto rDesc = dynamic_cast<NumberDescriptor *>(rValue.get());
		if(slDesc != nullptr && srDesc != nullptr) // we have two strings
			return stringOperations(slDesc, srDesc, token());
		else if(lDesc != nullptr && rDesc != nullptr) { // we have two numbers
			if(token().isRelOp())	
				return relOperations(lDesc, rDesc, token());
			else if(token().isArithmeticOperator())
				return arithOperations(lDesc, rDesc, token());
			else {
				std::cout << "InfixExprNode::evaluate: don't know how to ";
				std:: cout << "evaluate this operator\n";
				token().print();
				std::cout << std::endl;
				exit(2);
			}
		} else {
			std::cout << "InfixExprNode::evaluate invalid cast\n";
			exit(1);
		}
	}
	return nullptr; // Should never hit here
}

void InfixExprNode::print() {
    _left->print();
	std::cout << ' ';
    token().print();
	std::cout << ' ';
    _right->print();
	std::cout << std::endl;
}

// WholeNumber
WholeNumber::WholeNumber(Token token): ExprNode{std::move(token)} {}

void WholeNumber::print() {
    token().print();
}

std::shared_ptr<TypeDescriptor> WholeNumber::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
#ifdef DEBUG
    std::cout << "WholeNumber::evaluate: returning ";
	std::cout << token().getWholeNumber() << std::endl;
#endif
	std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
	desc->value.intValue = token().getWholeNumber();
    return desc;
}

// Float
Float::Float(Token token): ExprNode{std::move(token)} {}

void Float::print() {
	token().print();
}

std::shared_ptr<TypeDescriptor> Float::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
#ifdef DEBUG
    std::cout << "Float::evaluate: returning ";
	std::cout << token().getFloat() << std::endl;
#endif
	std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::DOUBLE);
	desc->value.doubleValue = token().getFloat();
    return desc;
}

// Variable
Variable::Variable(Token token): ExprNode{std::move(token)} {}

void Variable::print() {
    token().print();
}

std::shared_ptr<TypeDescriptor> Variable::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
#ifdef DEBUG
    std::cout << "Variable::evaluate: returning ";
	printValue( symTab.getValueFor(token().getName()).get() );
	std::cout << std::endl;
#endif
    return symTab.getValueFor(token().getName());
}

// String
String::String(Token token): ExprNode{std::move(token)} {}

void String::print() {
	token().print();
}

std::shared_ptr<TypeDescriptor> String::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
#ifdef DEBUG
    std::cout << "String::evaluate: returning " << token().getString() << std::endl;
#endif
	std::shared_ptr<StringDescriptor> desc =
		std::make_shared<StringDescriptor>(TypeDescriptor::STRING);
	desc->value = token().getString();
    return desc;
}

//Array
Array::Array(std::vector<std::shared_ptr<ExprNode>> list) :
	_list{list} {}

void Array::print() {
	for(auto &l: _list) {
		if(l != nullptr)
			l->print();
		std::cout << std::endl;
	}
}

std::shared_ptr<TypeDescriptor> Array::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
    //case with empty array
    if(_list.size() == 0 || _list[0] == nullptr)
		return std::make_shared<TypeDescriptor>(TypeDescriptor::NULLARRAY);
	//case with non-empty array
	else if(_list.size() > 0 && _list[0] != nullptr) {
		auto type = _list[0]->evaluate(symTab, funcTab)->type();
		if(type == TypeDescriptor::STRING) {
			std::shared_ptr<StringArray> sArrayDesc =
				std::make_shared<StringArray>(StringArray::STRINGARRAY);
			std::vector<std::string> _stringArray;
			for(unsigned i = 0; i < _list.size(); ++i) {
				auto sDesc = dynamic_cast<StringDescriptor*>
					(_list[i]->evaluate(symTab, funcTab).get());
				if(sDesc == nullptr) {
					std::cout << "Array::evaluate error: members must be of ";
					std::cout << "the same type\n";
					exit(1);
				} else
					_stringArray.push_back(sDesc->value);
            }
			sArrayDesc->stringArray = _stringArray;
			return sArrayDesc;
        } else if(type == TypeDescriptor::INTEGER) {
			std::shared_ptr<NumberArray> numArrayDesc =
				std::make_shared<NumberArray>(NumberArray::NUMBERARRAY);
			std::vector<int> _numberArray;
            //push items into number array
            for(unsigned i = 0; i < _list.size(); ++i) {
                auto nDesc = dynamic_cast<NumberDescriptor*>
					(_list[i]->evaluate(symTab, funcTab).get());
				if(nDesc == nullptr) {
					std::cout << "Array::evaluate error: members must be of ";
					std::cout << "the same type\n";
					exit(1);
				} else
					_numberArray.push_back(nDesc->value.intValue);
            }
            numArrayDesc->numberArray = _numberArray;
			return numArrayDesc;
        }
	}
	return nullptr;
}

//LenArray
LenArray::LenArray(Token token): ExprNode{std::move(token)} {}

void LenArray::print() {
	std::cout << "Array length";
}

std::shared_ptr<TypeDescriptor> LenArray::evaluate(SymTab & symTab, std::unique_ptr<FuncTab> &funcTab){
    
    std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);

	auto type = symTab.getValueFor(token().getName())->type();
	
    if(type == TypeDescriptor::NUMBERARRAY) {
        auto ndesc = dynamic_cast<NumberArray*>
			(symTab.getValueFor(token().getName()).get());
        desc->value.intValue = ndesc->nArraySize();
        return desc;
    } else if(type == TypeDescriptor::STRINGARRAY) {
        auto sdesc = dynamic_cast<StringArray*>
			(symTab.getValueFor(token().getName()).get());
		desc->value.intValue = sdesc->sArraySize();
        return desc;
    } else if(type == TypeDescriptor::NULLARRAY) {
		std::cout << "Hit type == TypeDescriptor::NULLARRAY\n";
		desc->value.intValue = 0;
        return desc;
	} else {
		std::cout << "Array length not supported for this type\n";
		exit(1);
	}
	return nullptr; // should never hit here
}

// Call 
CallExprNode::CallExprNode(std::string id,
						   std::vector<std::shared_ptr<ExprNode>> list) :
	_id{id}, _list{std::move(list)} {}

std::vector<std::shared_ptr<ExprNode>> & CallExprNode::list() {
	return _list;
}

std::string &CallExprNode::id() {
	return _id;
}

void CallExprNode::print() {
	std::cout << _id << std::endl;
	for(auto& l: _list) {
		l->print();
		std::cout << " ";
	}
	std::cout << std::endl;
}

std::shared_ptr<TypeDescriptor> CallExprNode::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
	if(!funcTab->isDefined( _id )){
		std::cout << "Function call error: " << _id << " is not defined.";
		std::cout << std::endl;
		exit(1);
	}
	auto function = funcTab->getFunction( _id );
	auto params = function->params();
	if (_list.size() != params.size()) {
		std::cout << "Function call error: incorrect number of arguments to";
		std::cout << " function: " << _id << std::endl;
		exit(1);
	}

	std::vector<std::shared_ptr<TypeDescriptor>> args;
	for(auto &l: _list)
		args.push_back(l->evaluate(symTab, funcTab));

	symTab.openScope(params, args);
	function->evaluate(symTab, funcTab);
	symTab.closeScope();

	if(symTab.isDefinedGlobal(RETURN)) {
		auto retVal = symTab.getValueFor(RETURN);
		symTab.removeReturn();
		return retVal;
	}
	return nullptr;
}

// Subscription
Subscription::Subscription(std::string id, std::unique_ptr<ExprNode> test) :
	_id{id}, _test{std::move(test)} {}

std::unique_ptr<ExprNode> &Subscription::test() {
	return _test;
}

std::string &Subscription::id() {
	return _id;
}

void Subscription::print() {
	std::cout << _id << std::endl;
	_test->print();
	std::cout << std::endl;
}

std::shared_ptr<TypeDescriptor> Subscription::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
	if(_test == nullptr) {
		std::cout << "_test is null\n";
		exit(1);
	}
	auto test = dynamic_cast<NumberDescriptor*>
		(_test->evaluate(symTab, funcTab).get());
	if(test == nullptr || test->type() != TypeDescriptor::INTEGER) {
		std::cout << "Subscription::evaluate error index is not an integer\n";
		exit(1);
	}
	int index = test->value.intValue;
	auto type = symTab.getValueFor(_id)->type();
	if(type == TypeDescriptor::NUMBERARRAY) {
		std::shared_ptr<NumberDescriptor> desc =
			std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
        auto narray = dynamic_cast<NumberArray*>
			(symTab.getValueFor(_id).get());
		int element = narray->nSub(index);
        desc->value.intValue = element;
        return desc;
    } else if(type == TypeDescriptor::STRINGARRAY) {
		std::shared_ptr<StringDescriptor> desc =
			std::make_shared<StringDescriptor>(TypeDescriptor::STRING);        
        auto sarray = dynamic_cast<StringArray*>
			(symTab.getValueFor(_id).get());
        std::string element = sarray->sSub(index);
        desc->value = element;
        return desc;
    } else if (type == TypeDescriptor::NULLARRAY) {
		std::cout << "Subscription::evaluate error index out of bounds\n";
		exit(1);
	}
	return nullptr;
}
