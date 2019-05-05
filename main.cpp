/*
 * Created by Tyler Gearing 3/14/19
 *
 */

#include "Parser.hpp"

#include <fstream>

int main(int argc, char *argv[]) {

    if( argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    std::ifstream inputStream;

    inputStream.open(argv[1], std::ios::in);
    if( ! inputStream.is_open()) {
        std::cout << "Unable to open " << argv[1] << ". Terminating...\n";
        exit(2);
	}

    Tokenizer tokenizer(inputStream);

    /*Token tok = tokenizer.getToken();
	while( !tok.eof() )
		tok = tokenizer.getToken();
		tokenizer.printProcessedTokens();*/
		
	Parser parser(tokenizer);
	auto ast = parser.file_input();

	auto funcTab = ast->funcTab();
	auto statements = ast->stmts();

	SymTab symTab;

	statements->evaluate(symTab, funcTab);

	return 0;
}
