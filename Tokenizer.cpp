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
    while( (inStream.get(c) && isalnum(c))  || c == '_') {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
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

std::string Tokenizer::readString()
{

    std::string strValue = "";
    char c;
    while (inStream.get(c) && c != '"')
        strValue += c;

    if ( ! inStream.good())
    {
        std::cout << "Error reading string value\n";
        exit(1);
    }

    return strValue;

}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingNewLine{false} {Stack.push(0);}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    Token token;

    if (! inStream.good() )
    {
        token.Dedent();
        return lastToken;
    }

    while( inStream.get(c) && isspace(c) && c != '\n')
    {

        if (parsingNewLine)
        {

            inStream.putback(c);

            int NumIndents = 0;

            while( inStream.get(c) && isspace(c))
                NumIndents++;

            inStream.putback(c);
            parsingNewLine = false;

            // if the stack and number of indents is the same then
            // start reading in the inputs and get out
            if (Stack.size() == NumIndents)
            {
                inStream.get(c);
                break;
            }
            if ( Stack.size() >= 2 || NumIndents > 0)
            { // We have more indentation to read

                // More indents than in stack, need to put into stack
                if ( NumIndents > Stack.top())
                {
                    token.Indent();
                    Stack.push(NumIndents);
                }
                else if ( NumIndents < Stack.top())
                { // Need to fix Stack to be same # of indents

                    if ( ! NumIndents) // There aren't any indents...leave this loop
                        break;

                    // My fixer for Stack
                    while(! Stack.empty())
                    {
                        if (Stack.top() != NumIndents)
                            Stack.pop();
                        else
                        {
                            if ( ! Stack.top()) // if stack holds 0 indents
                                token.Dedent();
                            else // 0 indents
                            {
                                Stack.pop();
                                token.Dedent();
                            }
                            break;
                        }
                    }
                    if (Stack.empty() && NumIndents > 0)
                    {
                        std::cout << "Syntax Error: Indentation mismatch\n";
                        exit(1);
                    }
                } // The number on the stack and the number of indents match, continue the process
                else
                {
                    inStream.get(c);
                    break;
                }

                _tokens.push_back(token);
                return lastToken = token;
            }

        } // end of if parseNewLine

    } // end of while loop

    // We have more stuff to read, dedents
    if (Stack.size() > 1 && parsingNewLine)
    {
        inStream.putback(c);
        token.Dedent();
        _tokens.push_back(token);

        // pop off the remaining stuff since
        // we are trying to go backwards
        while (Stack.size() != 1)
            Stack.pop();

        return lastToken = token;

    }



    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    //    std::cout << "c = " << c << std::endl;

    char a;

    if( inStream.eof()) {
        token.eof() = true;
    }
    else if ( c == '#' )
    {
        std::string s;
        getline(inStream, s);

        inStream.get(c);
        if (c != '#')
        {
            inStream.putback(c);
        }

    }
    else if( c == '\n' ) {
        parsingNewLine = true;
        token.eol() = true;

        inStream.get(c);
        if (c == '\n') ;
        else
            inStream.putback(c);


    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } else if( c == '=' ) {
        inStream.get(a);

        if (a == '>' || a == '<' || a == '!') {
            std::cout << "Invalid syntax in input -> =" << a << " <-\n";
            exit(1);
        }
        else if (a == '=')
            token.Relsymbol("==");
        else {
            inStream.putback(a);
            token.symbol(c);
        }
    }
    else if ( c == '>' ) {
        inStream.get(a);

        if (a == '=')
            token.Relsymbol(">=");
        else {
            inStream.putback(a);
            token.Relsymbol(">");
        }
    }
    else if ( c == '<' ) {

        inStream.get(a);

        if (a == '=')
            token.Relsymbol("<=");
        else {
            inStream.putback(a);
            token.Relsymbol("<");
        }
    }
    else if ( c == '!' ) {
        inStream.get(a);
        if (a == '=')
            token.Relsymbol("!=");
        else {
            std::cout << "Invalid syntax in input -> !" << a << " <-\n";
            exit(1);
        }
    }
    else if( c == '+' || c == '-' || c == '*' || c == '%')
        token.symbol(c);
    else if (c == '/')
    {
        inStream.get(a);

        if (a == '/')
            token.Relsymbol("//");
        else {
            inStream.putback(a);
            token.symbol(c);
        }
    }
    else if( c == ';' || c == ':' || c == ',' || c == '_' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if( c == '{' || c == '}')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        std::string tokNM = readName();
        if (tokNM == "print" || tokNM == "for" || tokNM == "if" || tokNM == "in" || tokNM == "range" || tokNM == "elif" || tokNM == "else" || tokNM == "or" || tokNM == "not" || tokNM == "and") {
            token.setKeyWord( tokNM );
        }
        else {
            token.setName(tokNM);
        }
    }
    else if (c == '"')
    {
        token.setStr( readString() );
    }
    else {
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




