//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include "TypeDescriptor.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements) {
        s->print();
        std::cout << std::endl;
    }

}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {

    Token rTok = rhsExpression()->token();

    if ( rTok.isWholeNumber() )
    {
        int rhs = rhsExpression()->Intevaluate(symTab);
        symTab.setValueFor(lhsVariable(), rhs);
    }
    else if (rTok.isStr())
    {
        std::string rhs = rhsExpression()->Strevaluate(symTab);
        symTab.setValueFor(lhsVariable(), rhs);
    }
    else if (rTok.isName()) // Var = variable
    {
        TypeDescriptor* descriptor = symTab.getValueFor(rTok.getName());

        if (descriptor->type() == TypeDescriptor::INTEGER)
        {
            int rhs = rhsExpression()->Intevaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        }
        else if (descriptor->type() == TypeDescriptor::BOOL)
        {
            // BOOL STUFF HERE
            // bool rhs = rhsExpression()->Bolevaluate(symTab);
            // symTab.setValueFor(lhsVariable(), rhs);
        }
        else
        {
            std::string rhs = rhsExpression()->Strevaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        }
    }
    else
    {
        InfixExprNode* exprNode = dynamic_cast<InfixExprNode*> (rhsExpression());

        ExprNode* lhs = exprNode->left();

        if (lhs->token().isStr())
        {
            std::string rhs = rhsExpression()->Strevaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        }
        else if (lhs->token().isWholeNumber())
        {
            int rhs = rhsExpression()->Intevaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        }
        else
        {
            // TypeDescriptor* descriptor = symTab.getValueFor(lhs->token().getName());
            InfixExprNode* exprNode = dynamic_cast<InfixExprNode*> (rhsExpression());

            if ( exprNode->left()->token().isStr() )
            {
                std::string rhs = rhsExpression()->Strevaluate(symTab);
                symTab.setValueFor(lhsVariable(), rhs);
            }
            else if ( exprNode->left()->token().isWholeNumber() )
            {
                int rhs = rhsExpression()->Intevaluate(symTab);
                symTab.setValueFor(lhsVariable(), rhs);
            }
            else // This is a TypeDescriptor thing
            {

                TypeDescriptor* descriptor = symTab.getValueFor(lhs->token().getName());

                if ( descriptor->type() == TypeDescriptor::STRING && ! rhsExpression()->token().isRelationalOperator() )
                {
                    std::string rhs = rhsExpression()->Strevaluate(symTab);
                    symTab.setValueFor(lhsVariable(), rhs);
                }
                else if (descriptor->type() == TypeDescriptor::INTEGER && ! rhsExpression()->token().isRelationalOperator() )
                {
                    int rhs = rhsExpression()->Intevaluate(symTab);
                    symTab.setValueFor(lhsVariable(), rhs);
                }
                else
                {
                    // BOOL Stuff here
                    // bool rhs = rhsExpression()->Bolevaluate(symTab);
                    // symTab.setValueFor(lhsVariable(), rhs);

                }

            } // 2nd inner inner end of else block
        } // end of inner else block
    } // end of outer else block


}// end of function Assignment's evaluate

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
}

PrintStatement::PrintStatement() : exprNodes{nullptr} {}

//PrintStatement::PrintStatement(std::string rhsExpr, ExprNode* expr) : _rhsID{rhsExpr}, _exprValue{expr} {}

PrintStatement::PrintStatement(std::vector<ExprNode *> *list) : exprNodes{list} {}

//ExprNode* &PrintStatement::exprValue()
//{
//    return _exprValue;
//}

void PrintStatement::print() {
    //std::cout << "print " << _rhsID << std::endl;
}

void PrintStatement::evaluate(SymTab &symTab) {
//    TypeDescriptor* TokDesc = symTab.getValueFor(_rhsID);
//    NumberDescriptor* Toknum = dynamic_cast<NumberDescriptor*>( TokDesc );
//    if (Toknum == nullptr) {
//        // This wasn't an integer variable, only other option currently is a string
//        StringDescriptor* Tokstr = dynamic_cast<StringDescriptor*>( TokDesc );
//        std::string StrValue = Tokstr->strVal();
//        std::cout << StrValue << std::endl;
//
//    }
//    else // The variable was an integer
//    {
//        int IntValue = Toknum->value.intValue;
//        std::cout << IntValue << std::endl;
//    }

    for (int i = 0; i < exprNodes->size(); i++)
    {

        if (exprNodes[0][i]->token().isArithmeticOperator())
        {
            InfixExprNode* Typer = dynamic_cast<InfixExprNode*> (exprNodes[0][i]);

            auto type = Typer->left()->token();

            if (type.isWholeNumber())
                std::cout << Typer->Intevaluate(symTab) << std::endl;
            else if (type.isStr())
                std::cout << Typer->Strevaluate(symTab) << std::endl;
            else
            { //var

                TypeDescriptor* TokDesc = symTab.getValueFor(Typer->left()->token().getName());
                NumberDescriptor* Toknum = dynamic_cast<NumberDescriptor*>( TokDesc );
                if (Toknum == nullptr)
                    std::cout << exprNodes[0][i]->Strevaluate(symTab) << std::endl; // Takes care of ? arith ?
                else
                    std::cout << exprNodes[0][i]->Intevaluate(symTab) << std::endl;
            }

        }
        else if (exprNodes[0][i]->token().isName())
        {
            TypeDescriptor* TokDesc = symTab.getValueFor(exprNodes[0][i]->token().getName());
            NumberDescriptor* Toknum = dynamic_cast<NumberDescriptor*>( TokDesc );
            if (Toknum == nullptr) {
                // This wasn't an integer variable, only other option currently is a string
                StringDescriptor* Tokstr = dynamic_cast<StringDescriptor*>( TokDesc );
                std::string StrValue = Tokstr->strVal();
                std::cout << StrValue << std::endl;

            }
            else // The variable was an integer
            {
                int IntValue = Toknum->value.intValue;
                std::cout << IntValue << std::endl;
            }
        }
        else if (exprNodes[0][i]->token().isWholeNumber())
            std::cout << exprNodes[0][i]->Intevaluate(symTab) << std::endl;
        else
            std::cout << exprNodes[0][i]->Strevaluate(symTab) << std::endl;


    } // end of for loop

}

ForStatement::ForStatement() {}

ForStatement::ForStatement(Range *range, Statements *stmts)
{
    _range = range;
    _stmts = stmts;
}

void ForStatement::print() {
    std::cout << "for ";
    _range->token().print();
    std::cout << " in range( ";
    _range->print();
    std::cout << " ):\n";
    _stmts->print();

}

void ForStatement::evaluate(SymTab &symTab) {

//    // evaluating initializing assignment
//    int rhs = _inital->rhsExpression()->Intevaluate(symTab);
//    symTab.setValueFor(_inital->lhsVariable(), rhs);
//
//    // evaluating condition expression
//    while (_condition->Intevaluate(symTab))
//    {
//        // evaluating increment assignment
//        int rhs = _increment->rhsExpression()->Intevaluate(symTab);
//        symTab.setValueFor(_increment->lhsVariable(), rhs);
//
//        // evaluating body statements
//        _body->evaluate(symTab);
//
//    } // loop until the condition is settled

    _range->initalizeIter(symTab);

    while (_range->Intevaluate(symTab))
        _stmts->evaluate(symTab);

}

IfStatement::IfStatement() : _stmts{nullptr} {}

IfStatement::IfStatement(std::vector<Statements *> *stmts, std::vector<ExprNode *> *nodes) : _stmts{stmts}, _nodes{nodes} {}

void IfStatement::print() {}

void IfStatement::evaluate(SymTab &symTab) {

    int NumNodes = _nodes[0].size();
    int i = 0;

    // For the number of things I have to print
    for (i = 0; NumNodes > 0; i++, NumNodes--)
    {
        // Check to see if I get a result of an integer, if so then
        // we need to evaluate a statement
        if (_nodes[0][i]->Intevaluate(symTab))
        {
            _stmts[0][i]->evaluate(symTab);
            break;
        }
    }

    // Forgot a node...oops
    if (i != _stmts->size() && NumNodes < 0)
        _stmts[0][(_stmts->size())-1]->evaluate(symTab);


}

