//
// Created by Johnathan Soto on 2019-03-25.
//

#include "TypeDescriptor.hpp"
#include <string.h>
#include <stdio.h>
#include <iostream>


TypeDescriptor::TypeDescriptor(types type): _type{type} {}

TypeDescriptor::types &TypeDescriptor::type() { return _type; }

NumberDescriptor::NumberDescriptor(types descType): TypeDescriptor{descType} {}

StringDescriptor::StringDescriptor(types descType, std::string val): TypeDescriptor{descType}, strValue{val} {}

std::string StringDescriptor::strVal() { return strValue;}



