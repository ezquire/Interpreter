//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor * InfixExprNode::evaluate(SymTab &symTab) {

  
    // Evaluates an infix expression using a post-order traversal of the expression tree.
   /* int lValue = left()->evaluate(symTab);
   int rValue = right()->evaluate(symTab);*/
  if((left()->evaluate(symTab)->type() == TypeDescriptor::INTEGER && right()->evaluate(symTab)->type() == TypeDescriptor::STRING) ||
    (left()->evaluate(symTab)->type() == TypeDescriptor::STRING && right()->evaluate(symTab)->type() == TypeDescriptor::INTEGER)){
    std::cout<<"InfixExprNode::evaluate: can't perform operations on two different data types : ";
    if(left()->evaluate(symTab)->type() == TypeDescriptor::INTEGER){
         NumberDescriptor * lValue = dynamic_cast<NumberDescriptor *>(left()->evaluate(symTab));
      	 std::cout<<lValue->value.intValue<<" ";
    }
    else if(left()->evaluate(symTab)->type() == TypeDescriptor::STRING){
         StringDescriptor * lValue = dynamic_cast<StringDescriptor *>(left()->evaluate(symTab));
	 std::cout<<lValue->strValue<<" ";
    }
								     						    						    
    if(right()->evaluate(symTab)->type() == TypeDescriptor::INTEGER){
        NumberDescriptor * rValue = dynamic_cast<NumberDescriptor *>(right()->evaluate(symTab));
	std::cout<<rValue->value.intValue<<" ";
    }
    else if(right()->evaluate(symTab)->type() == TypeDescriptor::STRING){
        StringDescriptor * rValue = dynamic_cast<StringDescriptor *>(right()->evaluate(symTab));
	std::cout<<rValue->strValue<<" ";
    }
    std::cout<<std::endl;
    
    exit(1);
  }
  // case for not expressions
  else if(right()->evaluate(symTab)->type() == TypeDescriptor::INTEGER && left()  == nullptr){
      
      NumberDescriptor * rValue = dynamic_cast<NumberDescriptor *>(right()->evaluate(symTab));
      if( token().isNotOperator()){
          int newVal = !(rValue->value.intValue);
          NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
          NEWval->value.intValue = newVal;
          return NEWval;
      }
      
  }
  else if(left()->evaluate(symTab)->type() == TypeDescriptor::INTEGER && right()->evaluate(symTab)->type() == TypeDescriptor::INTEGER){

        NumberDescriptor * lValue = dynamic_cast<NumberDescriptor *>(left()->evaluate(symTab));
        NumberDescriptor * rValue = dynamic_cast<NumberDescriptor *>(right()->evaluate(symTab));
	
      if( token().isOrOperator()){
          int newVal = lValue->value.intValue || rValue->value.intValue;
          NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
          NEWval->value.intValue = newVal;
          return NEWval;
      }
      else if( token().isAndOperator()){
          int newVal = lValue->value.intValue && rValue->value.intValue;
          NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
          NEWval->value.intValue = newVal;
          return NEWval;
      }
        else if( token().isAdditionOperator() ){
           int newVal = lValue->value.intValue + rValue->value.intValue;
	         NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	         NEWval->value.intValue = newVal;
	         return NEWval;
        }
        else if(token().isSubtractionOperator()){
           int newVal = lValue->value.intValue - rValue->value.intValue;
           NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
           NEWval->value.intValue = newVal;
           return NEWval; 
        }
        else if(token().isMultiplicationOperator()){
           int newVal = lValue->value.intValue * rValue->value.intValue;
           NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
           NEWval->value.intValue = newVal;
           return	NEWval;
        }
        else if(token().isDivisionOperator()){
           int newVal = lValue->value.intValue / rValue->value.intValue;
           NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
           NEWval->value.intValue = newVal;
           return	NEWval;
        }
        else if( token().isModuloOperator() ){
           int newVal = lValue->value.intValue % rValue->value.intValue;
           NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
           NEWval->value.intValue = newVal;
           return	NEWval;
        }

        //evaluate relational expressions
        else if( token().isEquivalentOperator() ){
          if(lValue->value.intValue == rValue->value.intValue) {
	    NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
	    NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
    
        else if( token().isUnequivalentOperator() ){
          if(lValue->value.intValue != rValue->value.intValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	    NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
        else if( token().isGreaterThanOperator() ){
          if(lValue->value.intValue > rValue->value.intValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	    NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
        else if( token().isLessThanOperator() ){
          if(lValue->value.intValue < rValue->value.intValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	    NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
        else if( token().isGreaterThanOrEqualOperator() ){
          if(lValue->value.intValue >= rValue->value.intValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	    NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
        else if( token().isLessThanOrEqualOperator() ){
          if(lValue->value.intValue <= rValue->value.intValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
	    NEWval->value.intValue = 0;
            return NEWval;
	  }
        }
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
  }
  
  else if(left()->evaluate(symTab)->type() == TypeDescriptor::STRING && right()->evaluate(symTab)->type() == TypeDescriptor::STRING){

        StringDescriptor * lValue = dynamic_cast<StringDescriptor *>(left()->evaluate(symTab));
        StringDescriptor * rValue = dynamic_cast<StringDescriptor *>(right()->evaluate(symTab));
	
        if( token().isAdditionOperator() ){
	         std::string newVal = lValue->strValue + rValue->strValue;
           StringDescriptor * NEWval = new StringDescriptor(TypeDescriptor::STRING);
           NEWval->strValue = newVal;
           return NEWval;
        }
        //evaluate relational expressions                                                                                                                                                            
        else if( token().isEquivalentOperator() ){
          if(lValue->strValue == rValue->strValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 0;
            return NEWval;
          }
        }

        else if( token().isUnequivalentOperator() ){
          if(lValue->strValue != rValue->strValue) {
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 1;
            return NEWval;
          }
          else{
            NumberDescriptor * NEWval = new NumberDescriptor(TypeDescriptor::INTEGER);
            NEWval->value.intValue = 0;
            return NEWval;
          }
        }
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
	    exit(2);
	}
  }
  

  
}
void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}


void WholeNumber::print() {
    token().print();
}


TypeDescriptor * WholeNumber::evaluate(SymTab &symTab) {

  NumberDescriptor * desc = new NumberDescriptor(TypeDescriptor::INTEGER);
  desc->value.intValue = token().getWholeNumber();

  return desc;
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
  token().print();
  
}

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
  
    return symTab.getValueFor(token().getName());
}

// String

String::String(Token token): ExprNode{token} {}

void String::print() {
    token().print();
}

TypeDescriptor *String::evaluate(SymTab &symTab) {

  StringDescriptor * desc = new StringDescriptor(TypeDescriptor::STRING);
  desc->strValue = token().getString();

  return desc;

}
