
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
    while (tok.isName() ||  tok.eol()|| tok.indent() || tok.dedent()) {
  
      if(tok.indent())
	      tok =tokenizer.getToken();
      else if(tok.dedent())
          tok = tokenizer.getToken();
      else if (tok.isKeywordPrint()){
	     tokenizer.ungetToken();
	     PrintStatement * printStmt = printStatement();
	     stmts->addStatement(printStmt);
	     tok = tokenizer.getToken();
         }
      else if(tok.isKeywordIf()){
        tokenizer.ungetToken();
        IfStatement * ifStmt = ifStatement();
        stmts->addStatement(ifStmt);
        tok = tokenizer.getToken();
      }
      else if(tok.isKeywordFor()){
	     tokenizer.ungetToken();
	     ForStatement * forStmt = forStatement();
	     stmts->addStatement(forStmt);
         tok= tokenizer.getToken();
	     if( tok.eol())
	         tok = tokenizer.getToken();
      }
      else if(tok.eol()) {
	      tok = tokenizer.getToken();
          while(tok.eol())
             tok = tokenizer.getToken();
      }
      else{
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
        die("Parser::assignStatement", "Expected a name token, instead got", varName);
   
    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);
  
    
    Token tok = tokenizer.getToken();
    if(tok.isString())

	     return new AssignmentStatement(varName.getName(), tok.getString());
    else
      {
      	tokenizer.ungetToken();
    
         ExprNode *rightHandSideExpr = or_test();
    
         return new AssignmentStatement(varName.getName(), rightHandSideExpr);
	 }
}


PrintStatement *Parser::printStatement() {

  //std::vector<std::string> printVector;
  std::vector<ExprNode *> printVector;

  Token keyWord = tokenizer.getToken();
  if(!keyWord.isKeywordPrint())
    die("Parser::printStatement", "Expected a keyword, instead got", keyWord);

  //parse testlist to return vector of ExprNodes containing either a string/variable/wholenumber/expression 
  printVector = testList();

  //if next token isnt eol parser dies
  Token eol = tokenizer.getToken();
  if(!eol.eol())
    die("Parser::printStatement", "Expected eol character, instead got", eol);
    
  return new PrintStatement(printVector);

         
}

ForStatement *Parser::forStatement() {

  Statements * forVector = new Statements();
  
  std::vector<ExprNode *> vect;

  Token keyWord = tokenizer.getToken();
  if(!keyWord.isKeywordFor())
    die("Parser::forStatement", "Expected a keyword token, instead got", keyWord);
  
  Token Var = tokenizer.getToken();
  if(!Var.isName())
  	die("Parser::forStatement", "Expected a name token, instead got", Var);

  Token In = tokenizer.getToken();
  if(In.getName() != "in")
  	die("Parser::forStatement", "Expected in token, instead go", In);

  Token Range = tokenizer.getToken();
  if(!Range.isKeywordRange())
  	die("Parser::forStatement", "Expected range token, instead go", Range);

  Token OpenParen = tokenizer.getToken();
  if(!OpenParen.isOpenParen())
  	die("Parser::forStatement", "Expected open parentheses token, instead go", OpenParen);

  vect = testList();

  Token CloseParen = tokenizer.getToken();
  if(!CloseParen.isCloseParen())
  	die("Parser::forStatement", "Expected closed parentheses token, instead go", CloseParen);

  Token Colon = tokenizer.getToken();
  if(!Colon.isColon())
  	die("Parser::forStatement", "Expected colon token, instead go", Colon);

  forVector = suite();

  //  tokenizer.ungetToken();

  return new ForStatement(vect,forVector);
  
  
}

IfStatement *Parser::ifStatement() {
   
    //ExprNode to hold first test
    ExprNode * test1;
    //Statements vector to hold each statement in if
    Statements * If;
    //ExprNode to hold elif test
    ExprNode * test2;
    //push all elif tests to vector to be evaluated
    std::vector<ExprNode *> test2Vect;
    //vector to hold all suites in elif statements
    std::vector<Statements*> _ElifVect;
    //ExprNode to hold elif test
    Statements * _elif;
    //vector to hold all statements in else
    Statements * _Else;

    Token keyWord = tokenizer.getToken();
    if(!keyWord.isKeywordIf())
        die("Parser::forStatement", "Expected a keyword if token, instead got", keyWord);
    
    test1 = or_test();
    
    Token Colon = tokenizer.getToken();
    if(!Colon.isColon())
        die("Parser::forStatement", "Expected colon token, instead go", Colon);
    
    If = suite();
    
    Token ElifTok = tokenizer.getToken();
 
       while(ElifTok.getName() == "elif"){
        
           test2 = or_test();
           test2Vect.push_back(test2);
        
           Token Colon2 = tokenizer.getToken();
           if(!Colon2.isColon())
               die("Parser::forStatement", "Expected colon token, instead go", Colon);
        
           _elif = suite();
        
           _ElifVect.push_back(_elif);

           ElifTok = tokenizer.getToken();
    }
    
    if(ElifTok.getName() == "else"){
        
        Token Colon = tokenizer.getToken();
        if(!Colon.isColon())
            die("Parser::forStatement", "Expected colon token, instead go", Colon);
        
        _Else = suite();
        
    }
    
    return new IfStatement(test1, If, test2Vect, _ElifVect, _Else);
    
}

Statements * Parser::suite() {

  Statements * Stmts = new Statements();

	Token NewLine = tokenizer.getToken();
	if(!NewLine.eol())
		die("Parser::suite", "Expected eol character, instead got", NewLine);

	Token Indent = tokenizer.getToken();
	if(!Indent.indent())
		die("Parser::suite", "Expected indent token, instead got", Indent);

	Token varName = tokenizer.getToken();
	if(!varName.isName())
		die("Parser::suite", "Expected name token, instead got", varName);


   while(varName.isName()|| varName.indent() || varName.eol()){
      if(varName.isKeywordPrint()){
        tokenizer.ungetToken();
        PrintStatement * printStmt = printStatement();
        Stmts->addStatement(printStmt);
        varName = tokenizer.getToken();
	}
    else if(varName.isKeywordFor()){
        tokenizer.ungetToken();
        ForStatement * forStmt = forStatement();
        Stmts->addStatement(forStmt);
        varName = tokenizer.getToken();
    }
    else if(varName.eol()||varName.indent())
        varName =tokenizer.getToken();
    else{
        tokenizer.ungetToken();
        AssignmentStatement * assignStmt = assignStatement();
        Stmts->addStatement(assignStmt);
        varName = tokenizer.getToken();			
       }
    }
 
      if(!varName.dedent())
        	die("Parser::suite", "Expected dedent token, instead got", varName);

      return Stmts;


}

//returns pointers to exprnodes
std::vector<ExprNode *> Parser::testList() {

  std::vector<ExprNode *> vect;
  //parses testlist
  ExprNode * left = or_test();
  //
  vect.push_back(left);
  Token tok = tokenizer.getToken();
  while(tok.isComma())
    {
      ExprNode * left2 = or_test();
      //
      vect.push_back(left2);
      tok = tokenizer.getToken();
    }
  tokenizer.ungetToken();
  return vect;
}

ExprNode *Parser::or_test(){
    
    ExprNode * left = and_test();
    Token tok = tokenizer.getToken();
    while(tok.isOrOperator()){
        InfixExprNode * p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::and_test(){
    
    ExprNode * left = not_test();
    Token tok = tokenizer.getToken();
    while(tok.isAndOperator()){
        InfixExprNode * p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::not_test(){
    
    ExprNode * left = relExpr();
    Token tok = tokenizer.getToken();
    if(tok.isNotOperator()){
        InfixExprNode * p = new InfixExprNode(tok);
        p->left() = nullptr;
        p->right() = relExpr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}
        


ExprNode *Parser::relExpr() {
    // This function parses the grammar rules:
    // <rel-expr> -> <rel-term> { (==,!=) <rel-term> }

    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isEquivalentOperator() || tok.isUnequivalentOperator() || tok.isGreaterThanOperator() || tok.isLessThanOperator() || tok.isGreaterThanOrEqualOperator() || tok.isLessThanOrEqualOperator()) {
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

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
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
    else if( tok.isString())
        return new String(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = relExpr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
