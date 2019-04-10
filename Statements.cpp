//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

range::range() {}

bool range::Range(int a, int b, int c){

  if(a >= b)
    return true;
  else if(a< b)
     return false;
}
// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr}, _rhsVariable{""} {}


AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}


AssignmentStatement::AssignmentStatement(std::string lhsVar, std::string rhsVar):
        _lhsVariable{lhsVar}, _rhsVariable{rhsVar} {}


void AssignmentStatement::evaluate(SymTab & symTab) {

  if(_rhsVariable.empty()){
    NumberDescriptor * rValue = dynamic_cast<NumberDescriptor *>(rhsExpression()->evaluate(symTab));
    int rhs = rValue->value.intValue;
    symTab.setValueFor(lhsVariable(), rhs);
  }
  else {
    symTab.setValueFor(lhsVariable(), rhsVariable());
  }
 
}
  
std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

std::string &AssignmentStatement::rhsVariable() {
  return _rhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {

  std::cout<< _lhsVariable << " = ";
  if(rhsVariable().empty()){
    _rhsExpression->print();
    std::cout<<std::endl;
  }
  else{
    std::cout<<'"'<< rhsVariable()<<'"';
    std::cout<<std::endl;
  }
}
//print statement when AssignmentStatment is called by ForStatment
//print statement without endl
void AssignmentStatement::printFor() {
  std::cout<< _lhsVariable << "=";
  _rhsExpression->print();
}
    

// PrintStatement

PrintStatement::PrintStatement() :  _StrVector {nullptr} {}

PrintStatement::PrintStatement(std::vector<ExprNode *> pVector) {

   _StrVector = pVector;
  
}

void PrintStatement::print() {
  
  std::cout<<"print ";
  for(int i = 0; i< _StrVector.size(); i++){
    _StrVector[i]->print();
    if(i < _StrVector.size() -1)
      std::cout<<",";
  }

  std::cout<<std::endl;
}

    
void PrintStatement::evaluate(SymTab & symTab) {

  //evaluate variables in print statement and outputs print statement
  for(auto s: _StrVector){
    TypeDescriptor * val = s->evaluate(symTab);
    if(val->type() == TypeDescriptor::INTEGER){
      NumberDescriptor * lval = dynamic_cast<NumberDescriptor *>(val);
      std::cout<<lval->value.intValue;
    }
    else if(val->type() == TypeDescriptor::STRING){
      StringDescriptor * lval = dynamic_cast<StringDescriptor *>(val);
      std::cout<<lval->strValue;
    }
    std::cout<<" ";
  }
  std::cout<<std::endl;
  std::cout<<"<PrintStatement::evaluate> statement executed on the console shown above"<<std::endl;
  
// ForStatement
}

ForStatement::ForStatement() : _statements {nullptr}, _forVector{nullptr} {}

ForStatement::ForStatement(std::vector<ExprNode *> fVector, Statements * stmts){
  _statements = stmts;
  _forVector = fVector;
}

void ForStatement::print(){}

void ForStatement::evaluate(SymTab & symTab){

  range * R= new range();
  std::vector<int> S;
  for(auto s: _forVector){
    TypeDescriptor * val = s->evaluate(symTab);
    if(val->type() == TypeDescriptor::INTEGER){
      NumberDescriptor * lval = dynamic_cast<NumberDescriptor *>(val);
      S.push_back(lval->value.intValue);
    }
    else if(val->type() == TypeDescriptor::STRING){
      std::cout<<"expected int type";
    }
  }
  if(S.size() == 3){
    R->setStart(S[0]);
    R->setEnd(S[1]-1);
    R->setStep(S[2]);
  }
  else if(S.size() == 2){
    R->setStart(S[0]);
    R->setEnd(S[1]-1);
    R->setStep(1);
  }
  else if(S.size() == 1){
    R->setStart(0);
    R->setEnd(S[0]-1);
    R->setStep(1);
  }
  else{
    std::cout<<"error range got wrong number of inputs"<<std::endl;
  }

  while(!R->Range(R->getStart(), R->getEnd(), R->getStep())){
      _statements->evaluate(symTab);
      
      R->setStart(R->getStart() + R->getStep());
    }
  //  delete R;
}

//If statement
IfStatement::IfStatement() : _test{nullptr}, _testVect{nullptr}, _if{nullptr}, _elif{nullptr}, _else{nullptr} {}

IfStatement::IfStatement(ExprNode * Test, Statements * If, std::vector<ExprNode *> Test2vect, std::vector<Statements * >Elif, Statements * Else){
    _test = Test;
    _testVect = Test2vect;
    _if = If;
    _elif = Elif;
    _else = Else;
}
 
void IfStatement::print(){}

void IfStatement::evaluate(SymTab & symTab){
    

    TypeDescriptor * val = _test->evaluate(symTab);
 
    NumberDescriptor * lval = dynamic_cast<NumberDescriptor *>(val);
 //   std::cout<<lval->value.intValue<<std::endl;
    
    if(lval->value.intValue == 1)
         _if->evaluate(symTab);
    else
      std::cout<<"if statement did not execute because test was not true"<<std::endl;
    /*
   int increment = 0;
   for(auto s: _testVect){
    TypeDescriptor * val1 = s->evaluate(symTab);

    NumberDescriptor * lval1 = dynamic_cast<NumberDescriptor *>(val1);


    if(lval1->value.intValue == 1)
        _elif[0]->evaluate(symTab);
    increment++;

   }

    
    _else->evaluate(symTab);
    */
}
    
        
    
    

