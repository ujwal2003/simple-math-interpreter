#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>

#include "tokens.h"

using namespace std;

class Lexer {
	private:
		string line;
		int lineIdx;
		char currChar;
		void advanceChar();
		
	public:
		Lexer(string inputLine);
		vector<Token> tokenize();
		Token createNumber();
		Token getOperatorOrParen();
		Token createVariable();
};

#endif