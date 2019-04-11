#ifndef __RANGE_HPP
#define __RANGE_HPP

#include "SymTab.hpp"
#include "Expr.hpp"

#include<vector>
#include<iostream> 

class Range {
public:
    Range();
	Range(int start, int end, int step);
	Range(std::string varName,
		  std::vector<std::unique_ptr<ExprNode>> rangeList,
		  SymTab &symTab);
	int getNext();
	bool atEnd();
	int &start();
	int &end();
	int &step();
	
private:
	int _start;
	int _end;
	int _step;	
};

#endif
