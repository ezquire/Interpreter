/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, bol{true}, col{0}, inStream{stream}, lastToken{}, stack{} { stack.push(0); }

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
// the first character in input is an alphabetic character.
// The function reads and returns all characters of the name.
std::string Tokenizer::readString() {
    std::string _string;
    char c;
    while( inStream.get(c) && c != '"')
        _string += c; 
    //if(inStream.good())  // In the loop, we have read one char too many.
	//	  inStream.putback(c);
    return _string;
}

// This function is called when it is known that
// the first character in input is a digit or a '.'
// The function reads and sets the appropriate number with the
// appropriate type.
void Tokenizer::readNumber(Token &tok) {
	char c;
	std::string input; // This string collects input
	bool isFloat = false; // set if we encounter a period character
	
	while( inStream.get(c) && ( isdigit(c) || c == '.' ) ) {
		if(c == '.' && isFloat) {
			std::cout << "Unexpected character in input. -> ";
			std::cout << c << " <-" << std::endl;
			exit(1);
		} else if( c == '.' )
			isFloat = true;
		input += c;
	}
	if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);

	// convert string to integer or double
	if( isFloat )
		tok.setFloat( stod(input) );
	else
		tok.setWholeNumber( stoi(input) );
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

// This function gets a token for the parser
// It determines the scoping by parsing whitespace
// and generating appropriate indent and dedent tokens
Token Tokenizer::getToken() {

    bool blankline = false;
    char c;
    Token token;

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
    if(lastToken.dedent() && col < stack.top() ) {
        stack.pop();
        token.dedent() = true;
        _tokens.push_back(token);
        return lastToken = token;
    } else if(lastToken.dedent() && col > stack.top() ) {
		std::cout << "Indentation error\n";
		exit(1);
	}
	if(bol) {
		// read tabs and spaces to determine indent/dedent tokens
		// blank lines and comment lines
		bol = false;
		col = 0;
		while( inStream.get(c) && c == ' ' && c != '\t' )
			col++;
		if( c == '\t') {
			blankline = true;
			while( inStream.get(c) && isspace(c) && c != '\n' )
				;
			if( c != '\n' && c != '#' ) {
				std::cout << "Encountered bad indent character\n";
				exit(1);
			}
		} else if( c == '\n' || c == '#' )
			blankline = true;
		
		if(inStream.good()) // Read one too many chars
			inStream.putback(c); // Put one back
		if( !blankline ) {
			if( col == stack.top() )
				;
			else if ( col > stack.top() ) {
				stack.push(col);
				token.indent() = true;
				_tokens.push_back(token);
				return lastToken = token; 
			} else if ( col < stack.top() ) {
			   //while( col < stack.top() ) {
                    stack.pop();
                    token.dedent() = true;
                    _tokens.push_back(token);
                    //bol = true;
			    //}
                return lastToken = token;
			} else {
				std::cout << "Error: inconsistent indentation.\n";
				exit(1);
			}
		}
	}
	if(inStream.bad()) {
		std::cout << "Error reading the input stream in Tokenizer.\n";
		exit(1);
	}
	// Skip spaces but not new-line chars.
	while( inStream.get(c) && isspace(c) && c != '\n' )
		 ;
	if( c == '#' ) // eat any characters followed by #
		while( inStream.get(c) && c != EOF && c != '\n' )
			;
	if( inStream.eof() )
		token.eof() = true; 
	else if( c == '\n') {
		token.eol() = true;
		bol = true;
	} else if( isdigit(c) || c == '.') {		
			// signed/unsigned integer, or double
			// put the digit back into the input stream so
			// we read the entire number in a function
			inStream.putback(c);
			readNumber(token);
	} else if( c == '=' ) {
		inStream.get(c);
		if(c != '=') { 
			token.symbol('=');
			inStream.putback(c); // we have read one too many characters
		} else
			token.relOp("==");
	} else if( c == '<' || c == '>' || c == '!') {
		inStream.putback(c);
		token.relOp( readOp() );
	} else if( c == '+' || c == '*' || c == '%' || c == '-')
		token.symbol(c);
	else if( c == '/' ) {
		inStream.get(c);
		if(c != '/') { 
			token.symbol('/');
			inStream.putback(c); // we have read one too many characters
		} else
			token.relOp("//");
	} else if( c == ':' )
		token.symbol(c);
	else if( c == '(' || c == ')' || c == '{' || c == '}' || c == ',')
		token.symbol(c);
	else if(isalpha(c)) {  // an identifier or string?
		// put c back into the stream so we can
		// read the entire name in a function.
		inStream.putback(c);
		token.setName( readName() );
	} else if(c == '"')
		token.setString( readString() );
	else {
		std::cout << "Unknown character in input. -> ";
		std::cout << c << " <-" << std::endl;
		exit(1);
	}
	_tokens.push_back(token);
	return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto const &t: _tokens) {
        t.print();
        std::cout << std::endl;
    }
}
