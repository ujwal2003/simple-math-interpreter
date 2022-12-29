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

//checks for atom ::= number | idt
ASTNode* Parser::atom(int backTrackIdx) {
	ASTNode* result = nullptr;
	
	//number
	if(currTok.type == T_Number) {
		result = new ASTNode(N_Number);
		result->init_NumberNode(currTok.value);
		nextToken();
		return result;
	}
	
	//variable
	else if(currTok.type == T_Variable) {
		result = new ASTNode(N_Variable);
		result->init_VariableNode(currTok.value);
		nextToken();
		return result;
	}
	
	//if neither number or variable, return null node and backtrack
	else {
		result = new ASTNode(N_NULL); 
		goToTokenAtIndex(backTrackIdx);
		return result;
	}
}

/*====expression rule methods====*/
//method for initial tokens
vector<ASTNode*> Parser::expr_infixExpr() {
	vector<ASTNode*> infixExprList;
	
	//unary operation
	if(currTok.type == T_Plus || currTok.type == T_Minus) {
		string sign = (currTok.type == T_Minus) ? "-" : "";
		nextToken();
		if(currTok.type == T_Number || currTok.type == T_Variable) {
			infixExprList.push_back(atom(tokenIdx));
			string temp = infixExprList[infixExprList.size()-1]->nodeValue;
			infixExprList[infixExprList.size()-1]->nodeValue = sign + temp;
			if(currTok.type != T_NONE) {
				//TODO: expr_restOfInfixExpr()
			}
		} else {
			//raise error
		}
	}
	
	//non-unary operation
	else if(currTok.type == T_Number || currTok.type == T_Variable) {
		infixExprList.push_back(atom(tokenIdx));
		if(currTok.type != T_NONE) {
			//TODO: expr_restOfInfixExpr()
		}
	}
	
	//error
	else {
		//raise error
	}
	
	return infixExprList;
}
/*==end expression rule methods==*/
#endif