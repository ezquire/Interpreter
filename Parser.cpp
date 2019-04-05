#include <vector>
#include <iostream>
#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"
#include "Range.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << " ";
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

	while( tok.eol() )
		tok = tokenizer.getToken();

	if( tok.indent() )
		die("Parser::statements", "Expected no indentation, instead got", tok);

    while ( tok.isName() ) {
		if( tok.isPrintKeyword() ) {
			tokenizer.ungetToken();
			PrintStatement *printStmt = printStatement();
			stmts->addStatement(printStmt);
			tok = tokenizer.getToken();
			if( tok.indent() )
				die("Parser::statements tok.isPrintKeyword()",
					"Expected no indentation, instead got", tok);
			while( tok.eol() ) // eat newlines after PrintStatement
				tok = tokenizer.getToken();
		} else if( tok.isForKeyword() ) {
			tokenizer.ungetToken();
			ForStatement *forStmt = forStatement();
			stmts->addStatement(forStmt);
			tok = tokenizer.getToken();
			while( tok.eol() ) // eat newlines after ForStatement block
				tok = tokenizer.getToken();
		} else if( tok.isIf() ) {
			tokenizer.ungetToken();
			IfStatement *ifStmt = ifStatement();
			stmts->addStatement(ifStmt);
			tok = tokenizer.getToken();
			while( tok.eol() ) // eat newlines after IfStatement block
				tok = tokenizer.getToken();
		} else {
			tokenizer.ungetToken();
			AssignmentStatement *assignStmt = assignStatement();
			stmts->addStatement(assignStmt);
			tok = tokenizer.getToken();
			if( tok.indent() )
				die("Parser::statements ",
					"Expected no indentation, instead got", tok);
			while( tok.eol() ) // eat newlines after AssignmentStatements
				tok = tokenizer.getToken();
		}
    }
	tokenizer.ungetToken();
    return stmts;
}


// Assignment statement parser
AssignmentStatement *Parser::assignStatement() {
    Token varName = tokenizer.getToken();

    if (!varName.isName())
        die("Parser::assignStatement",
			"Expected name token, instead got", varName);

    Token tok = tokenizer.getToken();
    if ( !tok.isAssignmentOperator() )
	  die("Parser::assignStatement",
		  "Expected assignment operator, instead got", tok);

    ExprNode *rightHandSideExpr = test();

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
	
	std::vector<ExprNode *>rightHandSideExpr = testlist();

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

	Token id = tokenizer.getToken();
    if ( !tok.isName() )
		die("Parser::forStatement", "Expected ID, instead got", tok);
		
	tok = tokenizer.getToken();
	if ( !tok.isInKeyword() )
		die("Parser::forStatement", "Expected 'in', instead got",tok);

	tok = tokenizer.getToken();
	if ( !tok.isRangeKeyword() )
		die("Parser::forStatement", "Expected 'range', instead got",tok);

 	tok = tokenizer.getToken();
    if ( !tok.isOpenParen() )
		die("Parser::forStatement", "Expected '(', instead got", tok);

 	std::vector<ExprNode *>rangeList = testlist();

	tok = tokenizer.getToken();
    if ( !tok.isCloseParen() )
		die("Parser::forStatement", "Expected ')', instead got", tok);

	tok = tokenizer.getToken();
    if ( !tok.isColon() )
		die("Parser::forStatement", "Expected ':', instead got", tok);

	Statements *stmts = suite();

	return new ForStatement(id.getName(), rangeList, stmts);
}


// IfStatement Parser
IfStatement *Parser::ifStatement() {
	
	Token tok = tokenizer.getToken();
	if( !tok.isIf() )
		die("Parser::ifStatement", "Expected 'if', instead got",tok);

	std::vector<ExprNode *> elifTests;
	std::vector<Statements *> elifSuites;
	Statements *elseSuite;
	
	ExprNode *firstTest = test();

	tok = tokenizer.getToken();
	if( !tok.isColon() )
		die("Parser::ifStatement", "Expected ':', instead got",tok);

	Statements *firstSuite = suite();

	tok = tokenizer.getToken();
	while( tok.isElif() ) {
		ExprNode *_test = test();
		elifTests.push_back(_test);
		tok = tokenizer.getToken();
		if( !tok.isColon() )
			die("Parser::ifStatement", "Expected ':', instead got",tok);
		Statements *_suite = suite();		
		elifSuites.push_back(_suite);		
		tok = tokenizer.getToken();
	}
	if( tok.isElse() ) {
		tok = tokenizer.getToken();
		if( !tok.isColon() )
			die("Parser::ifStatement", "Expected ':', instead got",tok);
		elseSuite = suite();
		return new IfStatement(firstTest, firstSuite, elifTests,
							   elifSuites, elseSuite); 
	}
	tokenizer.ungetToken();
	return new IfStatement(firstTest, firstSuite, elifTests,
						   elifSuites, nullptr);
}

Statements *Parser::suite() {
	// This function parses the grammar rules:
	// NEWLINE INDENT stmt+ DEDENT
 	Token tok = tokenizer.getToken();
	if ( !tok.eol() )
		die("Parser::suite", "Expected 'NEWLINE', instead got",tok);

	tok = tokenizer.getToken();
	while( tok.eol() )
		tok = tokenizer.getToken();

	if ( !tok.indent() )
		die("Parser::suite", "Expected 'INDENT', instead got",tok);
	
	Statements *_suite = statements();

	tok = tokenizer.getToken();	
	while( tok.eol() )
		tok = tokenizer.getToken();

	if( tok.eof() )
		return _suite;	
	else if ( !tok.dedent() )
		die("Parser::suite", "Expected 'DEDENT', instead got",tok);
	return _suite;
}

std::vector<ExprNode *>Parser::testlist() {
    // This function parses the grammar rules:
    // testlist: test {',' test}*
	std::vector<ExprNode *> list;
	
    ExprNode *first = test();
	list.push_back(first);
    Token tok = tokenizer.getToken();
    while( tok.isComma() ) {
		ExprNode *next = test();
		list.push_back(next);
		tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return list;
}

ExprNode *Parser::test() {
	// This function parses the grammar rules:
	// or_test: and_test {'or' and_test }*
    ExprNode *andtest = and_test();
	Token tok = tokenizer.getToken();
	while (tok.isOrKeyword()) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = andtest;
		p->right() = and_test();
		andtest = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return andtest;
}

ExprNode *Parser::and_test() {
	// This function parses the grammar rules:
	// and_test: not_test {'and' not_test}*
	ExprNode *nottest = not_test();
	Token tok = tokenizer.getToken();
	while (tok.isAndKeyword()) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = nottest;
		p->right() = not_test();
		nottest = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return nottest;
}

ExprNode *Parser::not_test() {
	// This function parses the grammar rules:
	// not_test: 'not' not_test | comparison
	Token tok = tokenizer.getToken();
	if( tok.isNotKeyword() ) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = not_test();;
		p->right() = nullptr;
		return p;
	} else {
		tokenizer.ungetToken();
		ExprNode *p = comparison();
		return p;
	}
}

ExprNode *Parser::comparison() {
    // This function parses the grammar rules:
    // comparison: arith_expr { comp_op  arith_expr }*
	// comp_op: < | > | == | >= | <= | <> | !=
    ExprNode *left = arith_expr();
    Token tok = tokenizer.getToken();
    while (tok.isLessThan() || tok.isGreaterThan()
		   || tok.isEqual() || tok.isGreaterThanEqual()
		   || tok.isLessThanEqual() || tok.isNotEqual() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arith_expr() {
    // This function parses the grammar rules:
	// arith_expr: term {( + | - ) term}*
    // However, it makes the '+' left associative.
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
    // term: factor {(* | / | % | //) factor}*
    // However, the implementation makes the '*' left associate.
    ExprNode *left = factor();
    Token tok = tokenizer.getToken();
    while (tok.isMultiplicationOperator()
		   || tok.isDivisionOperator()
		   || tok.isModuloOperator() || tok.isFloorDivision() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::factor() {
	// This function parses the grammar rules:
	// factor: {-}* factor | atom
	Token tok = tokenizer.getToken();
	if( tok.isSubtractionOperator() ) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = factor();
		p->right() = nullptr;
		return p;
	} else {
		tokenizer.ungetToken();
		ExprNode *p = atom();
		return p;
	}
}

ExprNode *Parser::atom() {
    // This function parses the grammar rules:
    // atom: ID | NUMBER | STRING | ( test )

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
        ExprNode *p = test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary",
				"Expected close-parenthesis, instead got", token);
        return p;
    }
	
	die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
