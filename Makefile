.SUFFIXES: .o .cpp .x

CXXFLAGS = -ggdb -Wall -Werror -Wpedantic -lm -std=c++17
GCC = g++

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)

all: statement.x

statement.x: $(objs)
	$(GCC) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *~ *.o *.x

.PHONY: all clean
