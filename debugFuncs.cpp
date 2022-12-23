#ifndef DEBUGFUNCS_CPP
#define DEBUGFUNCS_CPP

#include <iostream>
#include <vector>

#include "tokens.h"
#include "debugFuncs.h"

using namespace std;

string representToken(Token t) {
	switch(t) {
		case T_Number: return "NUMBER: " + t.value;
		case T_Variable: return "VAR: " + t.value;
		case T_Equal: return "EQUALS";
		case T_Plus: return "PLUS";
		case T_Minus: return "MINUS";
		case T_Multiply: return "MULT";
		case T_Divide: return "DIV";
		default: return "ERROR: UNKNOWN";
	}
}

void representTokenList(vector<Token> tokens, bool debugFlag) {
	if(debugFlag) {
		cout << '[';
		for(bool isFirst{true}; Token t: tokens) {
			cout << (isFirst ? isFirst = false, "" : ", ");
			cout << representToken(t);
		}
		cout << ']' << endl;
	}
}

#endif