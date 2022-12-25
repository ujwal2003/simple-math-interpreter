#ifndef DEBUGFUNCS_CPP
#define DEBUGFUNCS_CPP

#include <iostream>
#include <vector>

#include "tokens.h"
#include "debugFuncs.h"

using namespace std;

string representToken(Token t) {
	switch(t.type) {
		case T_Number: return "NUMBER: " + t.value;
		case T_Variable: return "VAR: " + t.value;
		case T_Equal: return "EQUALS";
		case T_OpenParen: return "OPEN_PAREN";
		case T_ClosedParen: return "CLOSE_PAREN";
		case T_Plus: return "PLUS";
		case T_Minus: return "MINUS";
		case T_Multiply: return "MULT";
		case T_Divide: return "DIV";
		default: return "ERROR: UNKNOWN";
	}
}

void representTokenList(vector<Token> tokens, bool debugFlag) {
	if(debugFlag) {
		cout << "DEBUG(tokens): " << '[';
		bool isFirst = true;
		for(Token t: tokens) {
			cout << (isFirst ? isFirst = false, "" : ", ");
			cout << representToken(t);
		}
		cout << ']' << endl;
	}
}

void representAST(ASTNode* root) {
	//null
	if(root == nullptr) {
		cout << "NULL";
		return;
	}
	//number node
	if(root->type == N_Number) {
		cout << root->nodeValue;
		return;
	}
	//variable node
	if(root->type == N_Variable) {
		cout << root->nodeValue;
		return;
	}
	//assign node
	if(root->type == N_Assign) {
		representAST(root->left); //get var
		cout << " = ";
		representAST(root->right); //get expression
		return;
	}
	//add node
	if(root->type == N_Add) {
		cout << '(';
		representAST(root->left);
		cout << " + ";
		representAST(root->right);
		cout << ')';
		return;
	}
	//subtract node
	if(root->type == N_Subtract) {
		cout << '(';
		representAST(root->left);
		cout << " - ";
		representAST(root->right);
		cout << ')';
		return;
	}
	//multiply node
	if(root->type == N_Multipy) {
		cout << '(';
		representAST(root->left);
		cout << " * ";
		representAST(root->right);
		cout << ')';
		return;
	}
	//divide node
	if(root->type == N_Divide) {
		cout << '(';
		representAST(root->left);
		cout << " / ";
		representAST(root->right);
		cout << ')';
		return;
	}
	//error
	else {
		cout << "{ERROR}";
		return;
	}
}
#endif