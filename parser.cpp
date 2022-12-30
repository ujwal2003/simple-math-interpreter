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
	
	if(currTok.type == T_Number || currTok.type == T_Variable)
		tree = expr();
	
	else if(currTok.type != T_NONE) {
		cout << "overflow of tokens, likely invalid syntax" << endl;
		raiseError();
	}
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
				expr_restOfInfixExpr(infixExprList);
			}
		} else {
			//raise error
			cout << "expected number or identifier after unary operation" << endl;
			raiseError();
		}
	}
	
	//non-unary operation
	else if(currTok.type == T_Number || currTok.type == T_Variable) {
		infixExprList.push_back(atom(tokenIdx));
		if(currTok.type != T_NONE) {
			expr_restOfInfixExpr(infixExprList);
		}
	}
	
	//error
	else {
		//raise error
		cout << "expected number or identifier at start of expression" << endl;
		raiseError();
	}
	
	return infixExprList;
}

//method for rest of the tokens in expression
void Parser::expr_restOfInfixExpr(vector<ASTNode*> &v) {
	Token prevToken;
	bool nextAtomIsNegative = false;
	prevToken.copyToken(Token(T_Number)); //initialize previous token as an atom
	
	while(currTok.type != T_NONE) {
		//valid syntax for operations
		if(isTokenIn(currTok, "+-*/") && (isAtom(prevToken) || prevToken.type == T_ClosedParen)) {
			v.push_back(returnOperationNodeFromToken(currTok));
			prevToken.copyToken(currTok);
			nextToken();
		}
		
		//unary operator syntax
		else if(isTokenIn(currTok, "+-") && isTokenIn(prevToken, "()")) {
			nextAtomIsNegative = true;
			prevToken.copyToken(currTok);
			nextToken();
		}
		
		//valid syntax for atoms
		else if(isAtom(currTok) && isTokenIn(prevToken, "+-*/(")) {
			if(nextAtomIsNegative) {
				prevToken.copyToken(currTok);
				v.push_back(atom(tokenIdx));
				string temp = v[v.size()-1]->nodeValue;
				v[v.size()-1]->nodeValue = "-" + temp;
				nextAtomIsNegative = false;
			} else {
				prevToken.copyToken(currTok);
				v.push_back(atom(tokenIdx));
			}
		}
		
		//valid syntax for open parenthesis
		else if(currTok.type == T_OpenParen && isTokenIn(prevToken, "+-*/(")) {
			if(prevToken.type == T_Minus && nextAtomIsNegative)
				nextAtomIsNegative = false;
			v.push_back(new ASTNode(N_dummyOpenParen));
			prevToken.copyToken(currTok);
			nextToken();
		}
		
		//valid syntax for closed parenthesis
		else if(currTok.type == T_ClosedParen && (isAtom(prevToken) || prevToken.type == T_ClosedParen)) {
			v.push_back(new ASTNode(N_dummyCloseParen));
			prevToken.copyToken(currTok);
			nextToken();
		}
		
		//error
		else {
			//raise error
			if(!v.empty()) {
				for(ASTNode* n: v)
					delete n;
			}
			cout << "invalid syntax for " << representToken(currTok) << endl;
			raiseError();
		}
	}
}

//checks if token is an atom
bool Parser::isAtom(Token t) {
	if(t.type == T_Number || t.type == T_Variable)
		return true;
	return false;
}

//checks if token type matches one of the operator characters in the given string
bool Parser::isTokenIn(Token t, string check) {
	for(char c: check) {
		switch(c) {
			case '+': if(t.type == T_Plus){return true;}
			case '-': if(t.type == T_Minus){return true;}
			case '*': if(t.type == T_Multiply){return true;}
			case '/': if(t.type == T_Divide){return true;}
			case '(': if(t.type == T_OpenParen){return true;}
			case ')': if(t.type == T_ClosedParen){return true;}
		}
	}
	return false;
}

//returns operation node based on token type
ASTNode* Parser::returnOperationNodeFromToken(Token t) {
	switch(t.type) {
		case T_Plus: return new ASTNode(N_Add);
		case T_Minus: return new ASTNode(N_Subtract);
		case T_Multiply: return new ASTNode(N_Multipy);
		case T_Divide: return new ASTNode(N_Divide);
		case T_OpenParen: return new ASTNode(N_dummyOpenParen);
		case T_ClosedParen: return new ASTNode(N_dummyCloseParen);
		default: return new ASTNode(N_NULL);
	}
}

//returns precedence of operator
int Parser::operatorPrecedence(NodeType n) {
	switch(n) {
		case N_Add: return 1;
		case N_Subtract: return 1;
		case N_Multipy: return 2;
		case N_Divide: return 2;
		default: return -1;
	}
}

//returns true if node type is operator
bool Parser::isOperator(NodeType n) {
	switch(n) {
		case N_Add: return true;
		case N_Subtract: return true;
		case N_Multipy: return true;
		case N_Divide: return true;
		default: return false;
	}
}

//changes infix expression into postfix
queue<ASTNode*> Parser::expr_shuntingYardAlgorithm(vector<ASTNode*> &v) {
	queue<ASTNode*> outputQueue;
	stack<ASTNode*> operatorStack;
	
	for(ASTNode* node: v) {
		if(node->type == N_Number || node->type == N_Variable) {
			outputQueue.push(node);
		}
		
		else if(isOperator(node->type)) {
			if(!operatorStack.empty() && isOperator(operatorStack.top()->type)) {
				while(operatorPrecedence(operatorStack.top()->type) > operatorPrecedence(node->type)) {
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
			}
			
			operatorStack.push(node);
		}
		
		else if(node->type == N_dummyOpenParen)
			operatorStack.push(node);
		
		else if(node->type == N_dummyCloseParen) {
			while(!operatorStack.empty() && operatorStack.top()->type != N_dummyOpenParen) {
				outputQueue.push(operatorStack.top());
				operatorStack.pop();
			}
			
			if(operatorStack.top()->type == N_dummyOpenParen) {
				operatorStack.pop();
			} else {
				for(int i=0; i<v.size(); i++)
					delete v[i];
				cout << "mismatched parenthesis error" << endl;
				raiseError();
			}
		}
	}
	
	while(!operatorStack.empty()) {
		outputQueue.push(operatorStack.top());
		operatorStack.pop();
	}
	
	return outputQueue;
}

//returns AST of expression
ASTNode* Parser::expr() {
	vector<ASTNode*> infixExpr = expr_infixExpr();
	queue<ASTNode*> postfixExpr = expr_shuntingYardAlgorithm(infixExpr);
	
	stack<ASTNode*> astStack;
	while(!postfixExpr.empty()) {
		if(postfixExpr.front()->type == N_Number || postfixExpr.front()->type == N_Variable) {
			astStack.push(postfixExpr.front());
			postfixExpr.pop();
		}
		
		if(isOperator(postfixExpr.front()->type)) {
			if(astStack.size() >= 2) {
				postfixExpr.front()->right = astStack.top();
				astStack.pop();
				postfixExpr.front()->left = astStack.top();
				astStack.pop();
				astStack.push(postfixExpr.front());
				postfixExpr.pop();
			} else {
				//error
				for(ASTNode* n: infixExpr)
					delete n;
				cout << "not enough arguments for operation" << endl;
				raiseError();
			}
		}
	}
	
	if(astStack.size() != 1) {
		for(ASTNode* n: infixExpr)
			delete n;
		cout << "failed to build abstract syntax tree for given expression" << endl;
		raiseError();
	}
	
	return astStack.top();
}
/*==end expression rule methods==*/
#endif