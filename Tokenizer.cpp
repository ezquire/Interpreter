//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

std::string Tokenizer::readString() {

  std::string str;
  char c;
  while( inStream.get(c) && c != '"' ) {
    str += c;
  }
 
  return str;
}

std::string Tokenizer::readEquiv() {

  std::string relop;
  char c;
  while( inStream.get(c) && c == '=' ) {
       relop += c;
  }
  if(inStream.good())
    inStream.putback(c);
  return relop;
}

std::string Tokenizer::readUnequiv() {
  std::string relop;
  char c;
  while( inStream.get(c) && c =='!' || c == '=' ) {
       relop += c;
  }
  if(inStream.good() )
    inStream.putback(c);
  return relop;
}

std::string Tokenizer::readGreater() {
  std::string relop;
  char c;
  while( inStream.get(c) && c== '>' || c == '='){
    relop +=c;
  }
  if(inStream.good() )
    inStream.putback(c);
  return relop;
}

std::string Tokenizer::readLess() {
  std::string relop;
  char c;
  while( inStream.get(c) && c == '<' || c == '='){
    relop += c;
  }
  if(inStream.good())
    inStream.putback(c);
  return relop;
}
    

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
  
    spaceCounter = 0;
    char c;
    Token token;

    //Handle indents and dedents
    while( inStream.get(c) && isspace(c) && c != '\n')
                spaceCounter+=1;

     if(parssingANewLine&&_indent.empty() && c != '\n'){

        token.indent() = true;
        parssingANewLine = false;

      }
    else if(parssingANewLine && _indent.size() > 0 &&c != '\n'){

          if(spaceCounter == _indent.top()){

             parssingANewLine = false;
          }
          else if(spaceCounter > _indent.top()){

            token.indent() = true;
            parssingANewLine = false;
          }
          else if(spaceCounter < _indent.top()) {

	    if(!_indent.empty()){    
              _indent.pop();
              token.dedent() = true;
	    }
          }

    }

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }


    if(token.dedent()){
       if(spaceCounter == 0){
	   if(inStream.good())
     	       inStream.putback(c);
	       } 
       else{
         for(int i = 0; i < spaceCounter; i++){
            if(inStream.good())
                  inStream.putback(c);
         }
      }
    }
    else if(token.indent()){
      _indent.push(spaceCounter);
      if(inStream.good())
           inStream.putback(c);
    }
   else if( inStream.eof()) {
     if(!_indent.empty()){
       token.dedent() = true;
       _indent.pop();
       if(inStream.good())
	 inStream.putback(c);
     }
     else{

       token.eof() = true;
     }
	  
   }
    else if( c == '#'){
      while(inStream.get(c) && c != '\n')
        ;
	    token.eol() = true;
    }
    else if( c == '\n' ) {
        token.eol() = true;
        parssingANewLine = true;
        
    }
    else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } else if( c == '=' ){
        char b = inStream.peek();
	if(b == '=') {
	  inStream.putback(c);
	  token.setEquiv( readEquiv() );
	}
	else
	  token.symbol(c);
    }
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == '!' ){
      char b = inStream.peek();
      if( b == '=') {
        inStream.putback(c);
        token.setEquiv( readUnequiv() );
      }
    }
    else if( c == '>'){
      char b = inStream.peek();
      if(b == '='){
	inStream.putback(c);
	token.setEquiv( readGreater() );
      }
      else{
	token.symbol(c);
      }
    }
    else if( c == '<' ){
      char b = inStream.peek();
      if( b == '=' ) {
	inStream.putback(c);
	token.setEquiv( readLess() );
      }
      else{
	token.symbol(c);
      }
    }
    else if( c == ',')
        token.symbol(c);
    else if( c == '{')
        token.symbol(c);
    else if( c == '}')
        token.symbol(c);
    else if( c == ';' )
        token.symbol(c);\
    else if( c == ':')
      token.symbol(c);
    else if( c == '"' ) {
         
      token.setString(readString());
    }
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
