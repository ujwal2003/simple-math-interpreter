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
		ASTNode* tree;
		
		void nextToken();
		void goToTokenAtIndex(int index);
		void raiseError();
		
	public:
		Parser(vector<Token> inTokenList);
		void parseAndConstructAST();
		ASTNode* getAST();
		
		ASTNode* atom(int backTrackIdx);
		vector<ASTNode*> expr_infixExpr();
		void expr_restOfInfixExpr(vector<ASTNode*> &v);
		bool isAtom(Token t);
		bool isTokenIn(Token t, string check);
		ASTNode* returnOperationNodeFromToken(Token t);
};

#endif