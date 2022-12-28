#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <vector>
#include "tokens.h"
#include "ast.h"
#include "error.h"

#include "parser.h"

using namespace std;

//advances to next token
void Parser::nextToken() {
	tokenIdx += 1;
	if(tokenIdx < tokens.size()) {
		currTok.copyToken(tokens[tokenIdx]);
	} else {
		currTok.copyToken(Token(T_NONE));
	}
}

//goes to token at particular index in tokens list
void Parser::goToTokenAtIndex(int index) {
	tokenIdx = index;
	if(tokenIdx < tokens.size()) {
		currTok.copyToken(tokens[tokenIdx]);
	} else {
		currTok.copyToken(Token(T_NONE));
	}
}

//constructor
Parser::Parser(vector<Token> inTokenList) {
	tree = nullptr;
	tokenIdx = -1;
	currTok.copyToken(Token(T_NONE));
	tokens = inTokenList;
	nextToken();
}

//main parse function
void Parser::parseAndConstructAST() {
	if(currTok.type == T_NONE)
		tree = nullptr;
}

//return constructed abstract syntax tree
ASTNode* Parser::getAST() {
	return tree;
}
#endif