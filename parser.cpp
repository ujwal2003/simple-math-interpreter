#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "tokens.h"
#include "ast.h"
#include "error.h"
#include "debugFuncs.h"

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

//error function
void Parser::raiseError() {
	deleteAST(tree);
	throw MathInterpreterError();
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

//creates unary operator nodes
ASTNode* Parser::unaryOperation() {
	stack<Token> unaryOpStack;
	queue<ASTNode*> resultQueue;
	
	while(currTok.type == T_Plus || currTok.type == T_Minus) {
		if(currTok.type == T_Plus) {
			unaryOpStack.push(currTok);
			nextToken();
		} else if(currTok.type == T_Minus) {
			unaryOpStack.push(T_Minus);
			nextToken();
		}
	}
	
	if(currTok.type == T_Number || currTok.type == T_Variable) {
		unaryOpStack.push(currTok);
		
		while(!unaryOpStack.empty()) {
			//variable
			if(unaryOpStack.top().type == T_Variable && resultQueue.empty()) {
				resultQueue.push(new ASTNode(N_Variable));
				resultQueue.front()->init_VariableNode(currTok.value);
				unaryOpStack.pop();
			}
			//number
			else if(unaryOpStack.top().type == T_Number && resultQueue.empty()) {
				resultQueue.push(new ASTNode(N_Number));
				resultQueue.front()->init_VariableNode(currTok.value);
				unaryOpStack.pop();
			}
			//plus
			else if(unaryOpStack.top().type == T_Plus && !resultQueue.empty()) {
				ASTNode* temp = resultQueue.front();
				resultQueue.pop();
				resultQueue.push(new ASTNode(N_UnaryOp));
				resultQueue.front()->init_UnaryOperator("+", temp);
				unaryOpStack.pop();
			}
			//minus
			else if(unaryOpStack.top().type == T_Minus && !resultQueue.empty()) {
				ASTNode* temp = resultQueue.front();
				resultQueue.pop();
				resultQueue.push(new ASTNode(N_UnaryOp));
				resultQueue.front()->init_UnaryOperator("-", temp);
				unaryOpStack.pop();
			}
			//error
			else {
				while(!resultQueue.empty()) {
					delete resultQueue.front();
					resultQueue.pop();
				}
				
				cout << "Invalid syntax for unary operations at token: " << representToken(unaryOpStack.top()) << endl;
				raiseError();
			}
		}
	} else {
		//raise error
		cout << "Invalid syntax for unary operations, expected a number or variable." << endl;
		raiseError();
	}
	
	if(resultQueue.size() != 1) {
		while(!resultQueue.empty()) {
			delete resultQueue.front();
			resultQueue.pop();
		}
		cout << "Error: could not construct abstract syntax tree for unary-operation, likely syntax error." << endl;
		raiseError();
	}
	
	return resultQueue.front();
}

//returns list of nodes
vector<ASTNode*> Parser::infixExpr() {
	vector<ASTNode*> infix;
	Token prevToken;
	prevToken.copyToken(Token(T_NONE));
	
	while(currTok.type != T_NONE) {
		//number
		if(currTok.type == T_Number || currTok.type == T_Variable) {
			prevToken.copyToken(currTok);
			infix.push_back(atom(tokenIdx)); //atom() advances to next token
		}
		//multiply
		else if(currTok.type == T_Multiply) {
			prevToken.copyToken(currTok);
			infix.push_back(new ASTNode(N_Multipy));
			nextToken();
		}
		//divide
		else if(currTok.type == T_Divide) {
			prevToken.copyToken(currTok);
			infix.push_back(new ASTNode(N_Divide));
			nextToken();
		}
		//open parenthesis
		else if(currTok.type == T_OpenParen) {
			prevToken.copyToken(currTok);
			infix.push_back(new ASTNode(N_dummyOpenParen));
			nextToken();
		}
		//closed parenthesis
		else if(currTok.type == T_OpenParen) {
			prevToken.copyToken(currTok);
			infix.push_back(new ASTNode(N_dummyCloseParen));
			nextToken();
		}
		//plus or minus
		else if(currTok.type == T_Plus || currTok.type == T_Minus) {
			if(prevToken.type == T_Plus || prevToken.type == T_Minus || prevToken.type == T_NONE) {
				infix.push_back(unaryOperation()); //unaryOperation() does not invoke nextToken()
				prevToken.copyToken(currTok);
				nextToken();
			} else {
				NodeType nType = (currTok.type == T_Plus) ? N_Add : N_Subtract;
				infix.push_back(new ASTNode(nType));
				prevToken.copyToken(currTok);
				nextToken();
			}
		}
		//error
		else {
			if(!infix.empty()) {
				for(ASTNode* n: infix) {
					delete n;
				}
			}
			infix.clear();
			cout << "invalid syntax for expression at token: " << representToken(currTok) << endl;
			raiseError();
		}
	}
	
	return infix;
}

//precedence
int Parser::opPrecedence(NodeType op) {
	switch(op) {
		case N_Add: return 1;
		case N_Subtract: return 1;
		case N_Multipy: return 2;
		case N_Divide: return 2;
		default: return -1;
	}
}

//left associativity
bool Parser::isLeftAssociative(NodeType op) {
	switch(op) {
		case N_Add: return true;
		case N_Subtract: return true;
		case N_Multipy: return true;
		case N_Divide: return true;
		default: return false;
	}
}

//converts infix expression to postfix
queue<ASTNode*> Parser::shuntingYardAlgorithm(vector<ASTNode*> infix) {
	queue<ASTNode*> outputQueue;
	stack<ASTNode*> operatorStack;
	
	//TODO: implementation
	
	return outputQueue;
}
#endif