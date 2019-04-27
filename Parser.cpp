/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Parser.hpp"

// Parser functions
void Parser::die(std::string const &where, std::string const &message, Token &token) {
    std::cout << where << " " << message << " ";
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been ";
	std::cout << "identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

std::unique_ptr<Statements> Parser::file_input() {
	// This function parses the grammar rule:
	// file_input: {NEWLINE | stmt}* ENDMARKER
	std::unique_ptr<Statements> stmts = std::make_unique<Statements>();
	Token tok = tokenizer.getToken();
	while( !tok.eof() ) {
		if ( tok.eol() )
			tok = tokenizer.getToken();
		else if( tok.isSimpleStatement() || tok.isCompoundStatement() ) {
            tokenizer.ungetToken();
            auto statement = stmt();
            stmts->addStatement(std::move(statement));
            tok = tokenizer.getToken();
        } else if( tok.indent() ) {
			std::cout << "Parser::file_input unexpected indent\n";
			exit(1);
		} else {
            tokenizer.ungetToken();
            return stmts;
        }
	}
	tokenizer.ungetToken();
	return stmts;
}

std::unique_ptr<Statement> Parser::stmt() {
	// This function parses the grammar rule:
	// stmt: simple_stmt | compound_stmt
	Token tok = tokenizer.getToken();
	if( tok.isSimpleStatement() ) {
		tokenizer.ungetToken();
		return std::move( simple_stmt() );
	} else if ( tok.isCompoundStatement() ) {
		tokenizer.ungetToken();
		return std::move( compound_stmt() );
	} else
		die("Parser::stmt", "Expected statement, instead got", tok);
	return nullptr; // should never reach here
}

std::unique_ptr<Statement> Parser::simple_stmt() {
	// This function parses the grammar rule:
	// simple_stmt: (print_stmt | assign_stmt | array_ops |
	//                call_stmt | return_stmt) NEWLINE
    Token tok = tokenizer.getToken();
	if( tok.isPrintKeyword() ) {
		tokenizer.ungetToken();
		auto print = print_stmt();
		tok = tokenizer.getToken();
		if(!tok.eol())
			die("Parser::simple_stmt", "Expected 'NEWLINE', instead got", tok);
		return std::move(print);
	} else if ( tok.isName() ) {
		tokenizer.ungetToken();
		auto assign = assign_stmt();
		tok = tokenizer.getToken();
		if(!tok.eol())
			die("Parser::simple_stmt", "Expected 'NEWLINE', instead got", tok);
		return std::move(assign);
	} else if ( tok.isArrayOp() ) {
		std::cout << "Not implemented yet" << std::endl;
		exit(1);
	} else if ( tok.isCall() ) {
		tokenizer.ungetToken();
		return std::move( call() );
	} else if ( tok.isReturnKeyword() ) {
		std::cout << "Not implemented yet" << std::endl;
		exit(1);
	} else
		die("Parser::simple_stmt", "Expected simple, instead got", tok);
	return nullptr; // should never reach here
}

std::unique_ptr<PrintStatement> Parser::print_stmt() {
	// This function parses the grammar rule:
	// print_stmt: 'print' [ testlist ]
	Token tok = tokenizer.getToken();	
    if ( !tok.isPrintKeyword() )
		die("Parser::print_stmt", "Expected 'print', instead got", tok);
	std::vector<std::shared_ptr<ExprNode>> rhsList = testlist();
	return std::make_unique<PrintStatement>(rhsList);
}

std::unique_ptr<AssignmentStatement> Parser::assign_stmt() {
	// This function parses the grammar rule:
	// assign_stmt: ID = ( test | array_init )
    Token varName = tokenizer.getToken();
	if (!varName.isName())
        die("Parser::assign_stmt", "Expected name, instead got", varName);
	Token tok = tokenizer.getToken();
	if ( !tok.isAssignmentOperator() )
		die("Parser::assign_stmt", "Expected '=', instead got", tok);
	auto expr = test();
	return std::make_unique<AssignmentStatement>(varName.getName(),
												 std::move(expr));
}

std::unique_ptr<Statement> Parser::compound_stmt() {
	// This function parses the grammar rule:
	// compound_stmt: if_stmt | for_stmt | func_def
	Token tok = tokenizer.getToken();
	if( tok.isForKeyword() ) {
		tokenizer.ungetToken();
		return std::move( for_stmt() );
	} else if( tok.isIf() ) {
		tokenizer.ungetToken();
		return std::move( if_stmt() );
	} else if( tok.isDefKeyword() ) {
		tokenizer.ungetToken();
		return std::move( func_def() );
	} else
		die("Parser::compound_stmt", "Expected compound, instead got", tok);
	return nullptr; // should never reach here
}

std::unique_ptr<CallStatement> Parser::call() {
	// This functions parses the grammar rule:
	// call: ID '(' { testlist } ')'
	Token tok = tokenizer.getToken();
	if( !tok.isCall() )
		die("Parser::call", "Expected ID '(', instead got", tok);
	std::string id = tok.getName();
	auto list = testlist();
	tok = tokenizer.getToken();
	if( !tok.isCloseParen() )
		die("Parser::call", "Expected ')', instead got", tok);
	return std::make_unique<CallStatement>(id, list);
}

std::unique_ptr<ExprNode> Parser::return_stmt() {
	// This function parses the grammar rule:
	// return_stmt: 'return' [test]
	Token tok = tokenizer.getToken();
	if(!tok.isReturnKeyword())
		die("Parser::return_stmt", "Expected 'return', instead got", tok);
	return std::move( test() );
}

std::vector<std::shared_ptr<ExprNode>> Parser::testlist() {
    // This function parses the grammar rule:
    // testlist: test {',' test}*
	std::vector<std::shared_ptr<ExprNode>> list;
    auto first = test();
	list.push_back(std::move(first));
    Token tok = tokenizer.getToken();
    while( tok.isComma() ) {
		auto next = test();
		list.push_back(std::move(next));
		tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return list;
}

std::unique_ptr<Statements> Parser::suite() {
	// This function parses the grammar rules:
	// NEWLINE INDENT stmt+ DEDENT
	Token tok = tokenizer.getToken();
	if ( !tok.eol() )
		die("Parser::suite", "Expected 'NEWLINE', instead got",tok);
	tok = tokenizer.getToken();
	while(tok.eol())
		tok = tokenizer.getToken();
	if ( !tok.indent() )
		die("Parser::suite", "Expected 'INDENT', instead got",tok);
	auto _suite = file_input();
	tok = tokenizer.getToken();
	while(tok.eol())
		tok = tokenizer.getToken();
	if( tok.eof() )
		return _suite;
	else if ( !tok.dedent() )
		die("Parser::suite", "Expected 'DEDENT', instead got",tok);
	return _suite;
}

std::unique_ptr<Statements> Parser::func_suite() {
	// This function parses the grammar rules:
	// NEWLINE INDENT (stmt | return_stmt)+ DEDENT
 	Token tok = tokenizer.getToken();
	if ( !tok.eol() )
		die("Parser::suite", "Expected 'NEWLINE', instead got",tok);
	tok = tokenizer.getToken();
	while(tok.eol())
		tok = tokenizer.getToken();
	if ( !tok.indent() )
		die("Parser::suite", "Expected 'INDENT', instead got",tok);
    auto _suite = file_input();
	tok = tokenizer.getToken();
	while(tok.eol())
		tok = tokenizer.getToken();
	if( tok.eof() )
	   return _suite;
	else if ( !tok.dedent() )
		die("Parser::suite", "Expected 'DEDENT', instead got",tok);
	return _suite;
}

std::unique_ptr<IfStatement> Parser::if_stmt() {
	// This function parses the grammar rule:
	// if_stmt: 'if' test ':' suite {'elif' test ':' suite}* ['else' ':' suite]
	Token tok = tokenizer.getToken();
	if( !tok.isIf() )
		die("Parser::if_stmt", "Expected 'if', instead got",tok);
	std::vector<std::unique_ptr<ExprNode>> elifTests;
	std::vector<std::unique_ptr<Statements>> elifSuites;
	auto firstTest = test();
	tok = tokenizer.getToken();
	if( !tok.isColon() )
		die("Parser::if_stmt", "Expected ':', instead got",tok);
	auto firstSuite = suite();
	tok = tokenizer.getToken();
	while( tok.isElif() ) {
		auto _test = test();
		elifTests.push_back(std::move(_test));
		tok = tokenizer.getToken();
		if( !tok.isColon() )
			die("Parser::if_stmt - elif", "Expected ':', instead got",tok);
		auto _suite = suite();		
		elifSuites.push_back(std::move(_suite));		
		tok = tokenizer.getToken();
	}
	if( tok.isElse() ) {
		tok = tokenizer.getToken();
		if( !tok.isColon() )
			die("Parser::if_stmt - else", "Expected ':', instead got",tok);
		auto elseSuite = suite();
		return std::make_unique<IfStatement>(std::move(firstTest),
											 std::move(firstSuite),
											 std::move(elifTests),
											 std::move(elifSuites),
											 std::move(elseSuite)); 
	} else {
	    tokenizer.ungetToken();
		return std::make_unique<IfStatement>(std::move(firstTest),
											 std::move(firstSuite),
											 std::move(elifTests),
											 std::move(elifSuites),
											 nullptr);
	}
}

std::unique_ptr<ForStatement> Parser::for_stmt() {
	// This function parses the grammar rule:
	// for_stmt: 'for' ID 'in' 'range' '(' testlist ')' ':' suite
    Token tok = tokenizer.getToken();
	if ( !tok.isForKeyword() )
		die("Parser::for_stmt", "Expected 'for', instead got", tok);
	Token id = tokenizer.getToken();
    if ( !id.isName() )
		die("Parser::for_stmt", "Expected ID, instead got", tok);
	tok = tokenizer.getToken();
	if ( !tok.isInKeyword() )
		die("Parser::for_stmt", "Expected 'in', instead got",tok);
	tok = tokenizer.getToken();
	if ( !tok.isRangeKeyword() )
		die("Parser::for_stmt", "Expected 'range', instead got",tok);
	tok = tokenizer.getToken();
    if ( !tok.isOpenParen() )
		die("Parser::for_stmt", "Expected '(', instead got", tok);
    auto rangeList = testlist();
	tok = tokenizer.getToken();
    if ( !tok.isCloseParen() )
		die("Parser::for_stmt", "Expected ')', instead got", tok);
	tok = tokenizer.getToken();
    if ( !tok.isColon() )
		die("Parser::for_stmt", "Expected ':', instead got", tok);
	auto _suite = suite();
	return std::make_unique<ForStatement>(id.getName(), std::move(rangeList),
										  std::move(_suite));
}

std::unique_ptr<Function> Parser::func_def() {
	// This function parses the grammar rule:
	// func_def: 'def' ID '(' [parameter_list] ')' ':' func_suite
	Token tok = tokenizer.getToken();
	if( !tok.isDefKeyword() )
		die("Parser::func_def", "Expected 'def', instead got", tok);
	Token id = tokenizer.getToken();
	if( !id.isName() )
		die("Parser::func_def", "Expected ID, instead got", tok);
	tok = tokenizer.getToken();
	if( !tok.isOpenParen() )
		die("Parser::func_def", "Expected '(', instead got", tok);
	auto param_list = parameter_list();
	tok = tokenizer.getToken();
	if( !tok.isCloseParen() )
		die("Parser::func_def", "Expected ')', instead got", tok);
	tok = tokenizer.getToken();
	if( !tok.isColon() )
		die("Parser::func_def", "Expected ':', instead got",tok);
	auto suite = func_suite(); 
	return std::make_unique<Function>(id.getName(), param_list,
									  std::move(suite));
}

std::vector<std::string> Parser::parameter_list() {
	// This function parses the grammar rule:
	// parameter_list: ID {, ID}*
	std::vector<std::string> list;
	Token tok = tokenizer.getToken();
	if(!tok.isName())
		die("Parser::parameter_list", "Expected ID, instead got",tok);
	list.push_back( tok.getName() );
	tok = tokenizer.getToken();
	while(tok.isComma()) {
		tok = tokenizer.getToken();
		if(!tok.isName())
			die("Parser::parameter_list", "Expected ID, instead got",tok);
		list.push_back( tok.getName() );
		tok = tokenizer.getToken();
	}
	if(tok.isName())
		list.push_back(tok.getName());
	else
		tokenizer.ungetToken();
	return list;
}

// Subscription

// Array init

// Array ops

// Array len

std::unique_ptr<ExprNode> Parser::test() {
	// This function parses the grammar rule:
	// or_test: and_test {'or' and_test }*
    auto andtest = and_test();
	Token tok = tokenizer.getToken();
	while (tok.isOrKeyword()) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
		p->left() = std::move(andtest);
		p->right() = and_test();
		andtest = std::move(p);
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return andtest;
}

std::unique_ptr<ExprNode> Parser::and_test() {
	// This function parses the grammar rules:
	// and_test: not_test {'and' not_test}*
	auto nottest = not_test();
	Token tok = tokenizer.getToken();
	while (tok.isAndKeyword()) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
		p->left() = std::move(nottest);
		p->right() = not_test();
		nottest = std::move(p);
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return nottest;
}

std::unique_ptr<ExprNode> Parser::not_test() {
	// This function parses the grammar rules:
	// not_test: 'not' not_test | comparison
	Token tok = tokenizer.getToken();
	if( tok.isNotKeyword() ) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
		p->left() = not_test();
		p->right() = nullptr;
		return p;
	} else {
		tokenizer.ungetToken();
		auto p = comparison();
		return p;
	}
}

std::unique_ptr<ExprNode> Parser::comparison() {
    // This function parses the grammar rules:
    // comparison: arith_expr { comp_op  arith_expr }*
	// comp_op: < | > | == | >= | <= | <> | !=
    auto left = arith_expr();
    Token tok = tokenizer.getToken();
    while (tok.isLessThan() || tok.isGreaterThan()
		   || tok.isEqual() || tok.isGreaterThanEqual()
		   || tok.isLessThanEqual() || tok.isNotEqual() ) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
        p->left() = std::move(left);
        p->right() = arith_expr();
        left = std::move(p);
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

std::unique_ptr<ExprNode> Parser::arith_expr() {
    // This function parses the grammar rules:
	// arith_expr: term {( + | - ) term}*
    // However, it makes the '+' left associative.
    auto left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
        p->left() = std::move(left);
        p->right() = term();
        left = std::move(p);
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

std::unique_ptr<ExprNode> Parser::term() {
    // This function parses the grammar rules:
    // term: factor {(* | / | % | //) factor}*
    // However, the implementation makes the '*' left associate.
    auto left = factor();
    Token tok = tokenizer.getToken();
    while (tok.isMultiplicationOperator()
		   || tok.isDivisionOperator()
		   || tok.isModuloOperator() || tok.isFloorDivision() ) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
        p->left() = std::move(left);
        p->right() = factor();
        left = std::move(p);
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

std::unique_ptr<ExprNode> Parser::factor() {
	// This function parses the grammar rules:
	// factor: {-}* factor | atom
	Token tok = tokenizer.getToken();
	if( tok.isSubtractionOperator() ) {
		std::unique_ptr<InfixExprNode> p =
			std::make_unique<InfixExprNode>(tok);
		p->left() = factor();
		p->right() = nullptr;
		return p;
	} else {
		tokenizer.ungetToken();
		auto p = atom();
		return p;
	}
}

std::unique_ptr<ExprNode> Parser::atom() {
    // This function parses the grammar rules:
    // atom: ID | NUMBER | STRING | ( test )

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return std::make_unique<WholeNumber>(tok);
    else if( tok.isFloat() )
		return std::make_unique<Float>(tok);
    else if( tok.isName() )
        return std::make_unique<Variable>(tok);
    else if( tok.isString() )
		return std::make_unique<String>(tok);
    else if (tok.isOpenParen()) {
        auto p = test();
        tok = tokenizer.getToken();
        if (!tok.isCloseParen())
            die("Parser::atom",
				"Expected close-parenthesis, instead got", tok);
        return p;
    }
	
	die("Parser::atom", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
