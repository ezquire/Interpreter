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
	} else { // We have a left() and a right()
		auto rValue = right()->evaluate(symTab, funcTab);
		auto slDesc = dynamic_cast<StringDescriptor *>(lValue.get());
		auto srDesc = dynamic_cast<StringDescriptor *>(rValue.get());
		auto lDesc = dynamic_cast<NumberDescriptor *>(lValue.get());
		auto rDesc = dynamic_cast<NumberDescriptor *>(rValue.get());

		if(slDesc != nullptr && srDesc != nullptr) // we have two strings
			return stringOperations(slDesc, srDesc, token());
		else if(lDesc != nullptr && rDesc != nullptr) { // we have two numbers
			if(token().isBooleanOperator()) { // Needs its own function? 
				auto lType = lDesc->type();
				if( token().isAndKeyword() ) {
					if(lType == TypeDescriptor::BOOLEAN) {
						if(lDesc->value.boolValue == 0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::DOUBLE) {
						if(lDesc->value.doubleValue == 0.0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::INTEGER) {
						if(lDesc->value.intValue == 0)
							return lValue;
						else return rValue;
					}
				} else if( token().isOrKeyword() ) {
					if(lType == TypeDescriptor::BOOLEAN) {
						if(lDesc->value.boolValue != 0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::DOUBLE) {
						if(lDesc->value.doubleValue != 0.0)
							return lValue;
						else return rValue;
					} else if(lType == TypeDescriptor::INTEGER) {
						if(lDesc->value.intValue != 0)
							return lValue;
						else return rValue;
					}
				}
			} else if(token().isRelOp())	
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
    token().print();
    _right->print();
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
		l->print();
		std::cout << std::endl;
	}
}

std::shared_ptr<TypeDescriptor> Array::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab) {
    //case with empty array
    if(_list.size() == 0)
		return std::make_shared<TypeDescriptor>(TypeDescriptor::NULLARRAY);
	//case with non-empty array
	else if(_list.size() > 0){
		//case for string arrays
		if(_list[0]->evaluate(symTab, funcTab)->type() == TypeDescriptor::STRING) {
            //create string array type descriptor
            std::shared_ptr<StringArray> desc =
               std::make_shared<StringArray>(StringArray::STRINGARRAY);
            //create vector to hold string array items
            std::vector<std::string> _stringArray;
            //push items into string array
            for(unsigned i = 0; i < _list.size(); ++i){
                auto sIDesc = dynamic_cast<StringDescriptor*>(_list[i]->evaluate(symTab, funcTab).get());
                _stringArray.push_back(sIDesc->value);
            }
            //set stringArray in String array descriptor to hold vector of strings
			desc->stringArray = _stringArray;
			return desc;
        }
        else if(_list[0]->evaluate(symTab, funcTab)->type() == TypeDescriptor::INTEGER){
            //create string array type descriptor
            
            std::shared_ptr<NumberArray> Ndesc =
              std::make_shared<NumberArray>(NumberArray::NUMBERARRAY);
            //create vector to hold string array items
            std::vector<int> _numberArray;
            //push items into string array
            
            for(unsigned i = 0; i < _list.size(); ++i) {
                auto sIDesc = dynamic_cast<NumberDescriptor*>(_list[i]->evaluate(symTab, funcTab).get());
                _numberArray.push_back(sIDesc->value.intValue);
            }
			//set numArray in String array descriptor to hold vector of strings
            Ndesc->numberArray = _numberArray;
			return Ndesc;
        }
	}
	return nullptr;
}

//LenArray
LenArray::LenArray(Token token): ExprNode{std::move(token)} {}

void LenArray::print() {}

std::shared_ptr<TypeDescriptor> LenArray::evaluate(SymTab & symTab, std::unique_ptr<FuncTab> &funcTab){
    
    std::shared_ptr<NumberDescriptor> desc =
		std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
    
    if(symTab.getValueFor(token().getName())->type() ==
	   NumberArray::NUMBERARRAY) {
        auto sIdesc = dynamic_cast<NumberArray*>
			(symTab.getValueFor(token().getName()).get());
        desc->value.intValue = sIdesc->nArraySize();
        return desc;
    } else if(symTab.getValueFor(token().getName())->type() ==
			StringArray::STRINGARRAY) {
        auto sIdesc = dynamic_cast<StringArray*>
			(symTab.getValueFor(token().getName()).get());
		desc->value.intValue = sIdesc->sArraySize();
        return desc;
    }
	return nullptr; // should never hit here
}

//PopArray
/*
PopArray::PopArray(Token token): ExprNode{std::move(token)} {}

void PopArray::print(){
    
    std::cout<<"arrayop";
}

std::shared_ptr<TypeDescriptor> PopArray::evaluate(SymTab &symTab, std::unique_ptr<FuncTab> &funcTab){
    
    if( !symTab.isDefined( token().getName() )) {
        std::cout << "Use of undefined variable, ";
        std::cout << token().getName() << std::endl;
        exit(1);
    }
    
    std::shared_ptr<NumberDescriptor> desc =
      std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
    
    if(symTab.getValueFor(token().getName())->type() == NumberArray::NUMBERARRAY){
        
        //dynamic_cast<NumberArray*>(symTab.getValueFor(token().getName()).get())->nPop();
        //sIdesc->nPop();
         
    }
    else if(symTab.getValueFor(token().getName())->type() == StringArray::STRINGARRAY){
        
        //dynamic_cast<StringArray*>(symTab.getValueFor(token().getName()).get())-> sPop();
        //sIdesc->sPop();
    }
    
    
    return desc;
    
    
}

*/

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
