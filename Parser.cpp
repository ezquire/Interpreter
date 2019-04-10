
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

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
    // a series of assignment statements.


    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while (tok.isName() || tok.isKeyWord()) {

        tokenizer.ungetToken();

        if (tok.isKeyWord())
        {
            if (tok.isPrint())
            {
                PrintStatement *printStmt = printStatement(); // print <id>
                stmts->addStatement(printStmt);
            }
            else if (tok.isFor() ){ //for statement
                ForStatement *forStmt = forStatement();
                stmts->addStatement(forStmt);
            }
            else
            {
                IfStatement *ifStmt = ifStatement();
                stmts->addStatement(ifStmt);
            }

        }
        else
        { // Do Assignments
            AssignmentStatement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
        }

        tok = tokenizer.getToken();

    }


    tokenizer.ungetToken();
    return stmts;
}

PrintStatement *Parser::printStatement() {

    std::vector<ExprNode*> *list = new std::vector<ExprNode*>;

    Token tok = tokenizer.getToken();

    if (!tok.isPrint())
        die("Parser::PrintStatement", "Expected print got -> ", tok);

    list->push_back(rel_expr()); // gets the first value (i.e. print ?)

    // ,
    tok = tokenizer.getToken();
    // Check to see if there is a comma, if so then more work
    while (tok.isComma())
    {
        list->push_back(rel_expr()); // get the next value
        tok = tokenizer.getToken();
    }

    if ( ! tok.eol() )
        die("Parser::PrintStatement", "Expected EOL got -> ", tok);

    return new PrintStatement(list);

}

// Exper. code
// Thinking:
//
//                                  and, not, or, ...
//                              /                       \
//                             /                         \
//                            /                           \
//                           /                             \
//              Relational Expression Node         Relational Expression Node
//
//std::vector<ExprNode*> Parser::testList()
//{
//
//    std::vector<ExprNode*> list;
//
//    ExprNode *left = test(); // return exprNode either a str, int, or variable
//
//    list.push_back(left);
//
//    Token token = tokenizer.getToken();
//
//    while (token.isComma())
//    {
//        list.push_back(test());
//        token = tokenizer.getToken();
//
//    }
//
//    tokenizer.ungetToken();
//
//    return list;
//
//}
//
//ExprNode *Parser::test()
//{
//
//    return or_test();
//
//}
//ExprNode *Parser::or_test()
//{
//
//    ExprNode* left = and_test();
//    Token token = tokenizer.getToken();
//
//    while (token.isOr())
//    {
//        InfixExprNode* p = new InfixExprNode(token);
//        p->left() = left;
//        p->right() = and_test();
//        left = p;
//        token = tokenizer.getToken();
//    }
//
//    tokenizer.ungetToken();
//
//    return left;
//
//}
//ExprNode *Parser::and_test()
//{
//
//    ExprNode* left = not_test();
//    Token token = tokenizer.getToken();
//
//    while (token.isAnd())
//    {
//        InfixExprNode* p = new InfixExprNode(token);
//        p->left() = left;
//        p->right() = not_test();
//        left = p;
//        token = tokenizer.getToken();
//    }
//
//    tokenizer.ungetToken();
//
//    return left;
//
//}
//ExprNode *Parser::not_test()
//{
//    ExprNode* left;
//    Token token = tokenizer.getToken();
//
//    if (token.isNot())
//        left = not_test();
//    else {
//        tokenizer.ungetToken();
//        left = comparison();
//    }
//
//    return left;
//
//}
//ExprNode *Parser::comparison()
//{
//
//    ExprNode* left = rel_expr();
//
//    Token token = tokenizer.getToken();
//
//    while (token.isRelationalOperator())
//    {
//        InfixExprNode *p = new InfixExprNode(token);
//        p->left() = left;
//        p->right() = rel_expr();
//        left = p;
//        token = tokenizer.getToken();
//    }
//
//    tokenizer.ungetToken();
//
//    return left;
//
//}





ForStatement *Parser::forStatement() {

    // This function parses the grammar rule:
    // <for-statement> -> for ( <assign-statement>; <rel-expr>; <assign-statement>) { <statements> }

    /*
     * for
     * ?
     * in
     * range
     * (
     * testlist
     * )
     * :
     * \n
     * INDENT
     * stmt(s)
     * DEDENT
     * */

    Token tok = tokenizer.getToken();

    // should pass this since it had to previously do to only 2 keywords presently
    // For
    if ( !tok.isFor() )
        die("Parser::forStatement", "Expected a for, got -> ", tok);

    // var, int...
    Token i = tokenizer.getToken();

    if ( ! i.isName() )
        die("Parser::forStatement", "Expected an name got -> ", tok);

    // Check for: in, range, (, ), :

    // in
    tok = tokenizer.getToken();
    if (! tok.isIn() )
        die("Parser::forStatement", "Expected 'in' got -> ", tok);

    // range
    tok = tokenizer.getToken();
    if ( ! tok.isRange() )
        die("Parser::forStatement", "Expected 'range' got -> ", tok);

    // (
    tok = tokenizer.getToken();
    if ( ! tok.isOpenParen() )
        die("Parser::forStatement", "Expected '(' got -> ", tok);

    // need the iterators, can be 1-3 args, so vector it
    std::vector<ExprNode*> *iterator = new std::vector<ExprNode*>;

    // Does 1-3 args for range
    for (int i = 0; i < 3; i++)
    {
        iterator->push_back(primary());
        tok = tokenizer.getToken();
        if ( !tok.isComma() )
            break; // only get here once if there is no comma

    } // still have next token in tok to utilize

    // Save all the range parameters
    Range *range = new Range(i, iterator); // storing the variable, int,...

    // )
    if ( ! tok.isCloseParen() )
        die("Parser::forStatement", "Expected ')' got -> ", tok);

    // :
    tok = tokenizer.getToken();
    if ( ! tok.isColon() )
        die("Parser::forStatement", "Expected ':' got -> ", tok);

    // \n
    tok = tokenizer.getToken();
    if ( ! tok.eol() )
        die("Parser::forStatement", "Expected '\n' got -> ", tok);

    // INDENT
    tok = tokenizer.getToken();
    if ( !tok.isIndent() )
        die("Parser::forStatement", "Expected 'INDENT' got -> ", tok);


    // Statements
    Statements *bodyStmts = statements();


    // DEDENT
    tok = tokenizer.getToken();
    if ( ! tok.isDedent() )
        die("Parser::forStatement", "Expected 'DEDENT' got -> ", tok);

//    // \n
//    tok = tokenizer.getToken();
//    if ( ! tok.eol() )
//        die("Parser::forStatement", "Expected 'newline' got -> ", tok);


    return new ForStatement(range, bodyStmts);


}

IfStatement *Parser::ifStatement() {

    /* if
     * test
     * :
     * \n
     * INDENT
     * statements
     * DEDENT
     * elif
     * test
     * :
     * \n
     * INDEDNT
     * Ststements
     * DEDENT
     * else
     * :
     * INDENT
     * Statements
     * DEDENT
     *
     * */

    // if
    Token tok = tokenizer.getToken();
    if ( !tok.isIf() )
        die("Parser::IfStatement", "Expected 'if' got -> ", tok);

    std::vector<Statements*> *stmts = new std::vector<Statements*>;
    std::vector<ExprNode*> *test = new std::vector<ExprNode*>;

    // gets the inside of the ()
    test->push_back(rel_expr());

    // :
    tok = tokenizer.getToken();
    if ( ! tok.isColon() )
        die("Parser::IfStatement", "Expected ':' got -> ", tok);

    // \n
    tok = tokenizer.getToken();
    if ( ! tok.eol() )
        die("Parser::IfStatement", "Expected '\n' got -> ", tok);

    // INDENT
    tok = tokenizer.getToken();
    if ( ! tok.isIndent() )
        die("Parser::IfStatement", "Expected 'INDENT' got -> ", tok);

    // statements
    stmts->push_back(statements());

    // DEDENT
    tok = tokenizer.getToken();
    if ( ! tok.isDedent() )
        die("Parser::IfStatement", "Expected 'DEDENT' got -> ", tok);

    // Check for elif and else
    tok = tokenizer.getToken();
    while ( tok.isElif() || tok.isElse() )
    {

        // Check for elif if so get the inside of ()
        if ( tok.isElif() )
            test->push_back(rel_expr());

        // :
        tok = tokenizer.getToken();
        if ( ! tok.isColon() )
            die("Parser::IfStatement", "Expected ':' got -> ", tok);

        // \n
        tok = tokenizer.getToken();
        if ( ! tok.eol() )
            die("Parser::IfStatement", "Expected '\n' got -> ", tok);

        // INDENT
        tok = tokenizer.getToken();
        if ( ! tok.isIndent() )
            die("Parser::IfStatement", "Expected 'INDENT' got -> ", tok);

        // statements
        stmts->push_back(statements());

        // DEDENT
        tok = tokenizer.getToken();
        if ( ! tok.isDedent() )
            die("Parser::IfStatement", "Expected 'DEDENT' got -> ", tok);

        tok = tokenizer.getToken();

    }

    tokenizer.ungetToken();

    return new IfStatement(stmts, test);




}

AssignmentStatement *Parser::assignStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = rel_expr();

    Token tok = tokenizer.getToken();
    if (!tok.eol())
        die("Parser::assignStatement", "Expected a newline, instead got", tok);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ExprNode *Parser::rel_expr() {
    // This function parses the grammar rules

    // <rel-expr> -> <expr> { ( ==, !=, >, >=, <, <=) <expr> }

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();

    while (tok.isEqualtoOperator() || tok.isNotEqualOperator() || tok.isGreaterOperator() || tok.isGreatEqualOperator() || tok.isLessOperator() || tok.isLessEqualOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return left;
}
/*
ExprNode *Parser::rel_term() {

    // This function parses the grammar rules
    // <rel-term> -> <rel-primary> {(>, >=, <, <=) <rel-primary>}

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();

    while ( tok.isGreaterOperator() || tok.isGreatEqualOperator() || tok.isLessOperator() || tok.isLessEqualOperator() ) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return left;

}
 */

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
    // <mult_op> -> * | / | % | //

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isIntDiv()) {
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
    else if( tok.isName() )
        return new Variable(tok);
    else if ( tok.isStr() )
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
