#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "tokens.h"
#include "ast.h"
#include "error.h"
#include "debugFuncs.h"

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
		
		ASTNode* unaryOperation();
		
		vector<ASTNode*> infixExpr();
		int opPrecedence(NodeType op);
		bool isLeftAssociative(NodeType op);
		bool greaterPrecOrSameAndLeftAssoc(NodeType topOfStack, NodeType currOp);
		queue<ASTNode*> shuntingYardAlgorithm(vector<ASTNode*> infix);
		ASTNode* expr();
		
		ASTNode* assign(int backTrackIdx);
};

#endif