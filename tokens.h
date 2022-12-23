#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>

using namespace std;

enum TokenType {
	T_Number,
	T_Variable,
	T_Equal,
	T_OpenParen,
	T_ClosedParen,
	T_Plus,
	T_Minus,
	T_Multiply,
	T_Divide,
};

struct Token {
	TokenType type;
	string value;
	
	Token(TokenType inType, string inVal="") {
		type = inType;
		value = inVal;
	}
};

#endif