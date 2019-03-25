#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of statements.

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while ( tok.isName() ) {
		if( tok.isPrintKeyword() ) {
			tokenizer.ungetToken();
			PrintStatement *printStmt = printStatement();
			stmts->addStatement(printStmt);
			tok = tokenizer.getToken();
		} else if( tok.isForKeyword() ) {
			tokenizer.ungetToken();
			ForStatement *forStmt = forStatement();
			stmts->addStatement(forStmt);
			tok = tokenizer.getToken();
		}
		else {
			tokenizer.ungetToken();
			AssignmentStatement *assignStmt = assignStatement();
			stmts->addStatement(assignStmt);
			tok = tokenizer.getToken();
		}
    }
	tokenizer.ungetToken();
    return stmts;
}

AssignmentStatement *Parser::assignStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected name token, instead got", varName);

    Token tok = tokenizer.getToken();
    if ( !tok.isAssignmentOperator() )
	  die("Parser::assignStatement", "Expected assignment operator, instead got", tok);

    ExprNode *rightHandSideExpr = rel_expr();

	tok = tokenizer.getToken();
	if( !tok.eol() )
		die("Parser::assignStatement", "Expected 'NEWLINE', instead got", tok);
	
    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

// Print statement parser
PrintStatement *Parser::printStatement() {
    Token tok = tokenizer.getToken();
    if ( !tok.isPrintKeyword() )
        die("Parser::PrintStatement", "Expected 'print', instead got", tok);
	
    ExprNode *rightHandSideExpr = rel_expr();

	tok = tokenizer.getToken();
	if( !tok.eol() )
		die("Parser::PrintStatement", "Expected 'NEWLINE', instead got", tok);
	
    return new PrintStatement(rightHandSideExpr);
}

// For statement parser
ForStatement *Parser::forStatement() {
    Token tok = tokenizer.getToken();
    if ( !tok.isForKeyword() )
        die("Parser::forStatement", "Expected 'for', instead got", tok);

	tok = tokenizer.getToken();
    if ( !tok.isOpenParen() )
		die("Parser::forStatement", "Expected '(', instead got", tok);
		
	AssignmentStatement *firstAssign = assignStatement();

	tok = tokenizer.getToken();
	if ( !tok.isSemiColon() )
		die("Parser::forStatement", "Expected ';', instead got",tok);

	ExprNode *midExpr = rel_expr();

	tok = tokenizer.getToken();
	if ( !tok.isSemiColon() )
		die("Parser::forStatement", "Expected ';', instead got",tok);
		
	AssignmentStatement *secondAssign = assignStatement();

	tok = tokenizer.getToken();
    if ( !tok.isCloseParen() )
		die("Parser::forStatement", "Expected ')', instead got", tok);


	tok = tokenizer.getToken();
	if ( !tok.eol() )
		die("Parser::forStatement", "Expected 'NEWLINE', instead got",tok);

	Statements *stmts = statements();

	return new ForStatement(firstAssign, midExpr, secondAssign, stmts);
}

/*ExprNode *Parser::testlist() {
    // This function parses the grammar rules:

    // <rel-expr> -> <rel-term> { (==, !=) <rel-term> }

    ExprNode *left = rel_term();
    Token tok = tokenizer.getToken();
    while (tok.isEqual() || tok.isNotEqual()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
	}*/

ExprNode *Parser::rel_expr() {
    // This function parses the grammar rules:

    // <rel-expr> -> <expr> { (<, >, ==, >=, <=, <>, !=) <expr> }

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isLessThan() || tok.isGreaterThan()
		   || tok.isEqual() || tok.isGreaterThanEqual()
		   || tok.isLessThanEqual() || tok.isNotEqual() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator()
		   || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
	else if( tok.isFloat() )
		return new Float(tok);
    else if( tok.isName() )
        return new Variable(tok);
	else if( tok.isString() )
		return new String(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = rel_expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
	
	die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
