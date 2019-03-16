//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

#define TABSIZE 8

// This function is called when it is known that
// the first character in input is an alphabetic character.
// The function reads and returns all characters of the name.
std::string Tokenizer::readName() {
    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c))
        name += c;
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

// This function is called when it is known that
// the first character in input is a digit.
// The function reads and returns the appropriate number with the
// appropriate type.
int Tokenizer::readInteger() {
    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) )
        intValue = intValue * 10 + c - '0';
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

// This function is called when it is known that
// the first character in input is a an operator.
// The function reads and returns the full operator.
std::string Tokenizer::readOp() {
    std::string op;
    char c;
	while(inStream.get(c) && (c == '<' || c == '>' || c == '=' || c == '!'))
		op += c;
	if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
	return op;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, bol{true}, inStream{stream}, lastToken{}, stack{}, altstack{} { stack.push(0), altstack.push(0); }

// This function gets a token for the parser
// It determines the scoping by parsing whitespace
// and generating appropriate indent and dedent tokens
Token Tokenizer::getToken() {

    if(ungottenToken) {
		ungottenToken = false;
		return lastToken;
	}

	char c;
	Token token;

	/*if(bol) {
	// read tabs and spaces to determine indent/dedent tokens
		int col = 0;
		bol = false;

		while( inStream.get(c) && ( c == ' ' || c == '\t') ) {
			if ( c == ' ' )
				col++;
			else if ( c == '\t' )
				col = ( col / TABSIZE + 1 ) * TABSIZE;
		}
		
		if( inStream.good() ) // Read one too many chars
			inStream.putback(c); // Put one character back

		if ( col > stack.top() ) {
			stack.push(col);
			token.indent() = true;
		    _tokens.push_back(token);
			return lastToken = token;
		}

		if ( col < stack.top() ) {
			while( col < stack.top() ) {
				stack.pop();
				token.dedent() = true;
				_tokens.push_back(token);
			}
			if( col != stack.top() ) {
				std::cout << "Error: inconsistent dedent.\n";
				exit(1);
			}
			return lastToken = token;
		}

	}*/	

	while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
		;

	if(inStream.bad()) {
		std::cout << "Error reading the input stream in Tokenizer.\n";
		exit(1);
	}

	if( c == '\n' ) {
		token.eol() = true;
		bol = true;
	}	
	else if( inStream.eof() )
		token.eof() = true;
	else if( isdigit(c) ) { // an integer, double
		// put the digit back into the input stream so
		// we read the entire number in a function




		inStream.putback(c);
		token.setWholeNumber( readInteger() );

	}
	else if( c == '=' ) {
		inStream.get(c);
		if(c != '=') { 
			token.symbol('=');
			inStream.putback(c); // we have read one too many characters
		}
		else
			token.relOp("==");
	}
	else if( c == '<' || c == '>' || c == '!') {
		inStream.putback(c);
		token.relOp( readOp() );
	}
	else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
		token.symbol(c);
	else if( c == ';' )
		token.symbol(c);
	else if( c == '(' || c == ')' || c == '{' || c == '}')
		token.symbol(c);
	else if(isalpha(c)) {  // an identifier?
		// put c back into the stream so we can
		// read the entire name in a function.
		inStream.putback(c);
		token.setName( readName() );
	}
	else {
		std::cout << "Unknown character in input. >" << c << "<" << std::endl;
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
