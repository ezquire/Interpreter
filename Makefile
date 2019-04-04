.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects =  Range.o Token.o Tokenizer.o Parser.o Expr.o SymTab.o Statements.o TypeDescriptor.o main.o

all: statement.x

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@
Range.o: Range.cpp Range.hpp
Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
Expr.o: Expr.cpp Expr.hpp Token.hpp SymTab.hpp
SymTab.o: SymTab.cpp SymTab.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp Expr.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp Expr.hpp Token.hpp SymTab.hpp Range.hpp
TypeDescriptor.o: TypeDescriptor.cpp TypeDescriptor.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp Expr.hpp Statements.hpp TypeDescriptor.hpp

clean:
	rm -fr *.o *~ *.x

.PHONY: all
