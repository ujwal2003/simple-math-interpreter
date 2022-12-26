#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include "tokens.h"
#include "ast.h"
#include "error.h"

using namespace std;

class Parser {
	private:
		int tokenIdx;
		Token currTok;
		vector<Token> tokens;
		
		void nextToken();
		void goToTokenAtIndex(int index);
		
	public:
		Parser(vector<Token> inTokenList);
		ASTNode* parseAndConstructAST();
};

#endif