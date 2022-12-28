#ifndef AST_CPP
#define AST_CPP

#include <iostream>
#include "ast.h"

using namespace std;

//constructor
ASTNode::ASTNode(NodeType initNodeType) {
	type = initNodeType;
	nodeValue = "";
	left = nullptr;
	right = nullptr;
}

//number node
void ASTNode::init_NumberNode(string number) {
	nodeValue = number;
}

//unary operator node
void ASTNode::init_UnaryOperatorNode(ASTNode* numVal, string sign) {
	left = numVal;
	nodeValue = sign;
}

//variable node
void ASTNode::init_VariableNode(string var) {
	nodeValue = var;
}

//assign node
void ASTNode::init_AssignNode(ASTNode* var, ASTNode* expression) {
	left = var;
	right = expression;
}

//add node
void ASTNode::init_AddNode(ASTNode* opLeft, ASTNode* opRight) {
	left = opLeft;
	right = opRight;
}

//subtract node
void ASTNode::init_SubtractNode(ASTNode* opLeft, ASTNode* opRight) {
	left = opLeft;
	right = opRight;
}

//multiply node
void ASTNode::init_MultiplyNode(ASTNode* opLeft, ASTNode* opRight) {
	left = opLeft;
	right = opRight;
}

//divide node
void ASTNode::init_DivideNode(ASTNode* opLeft, ASTNode* opRight) {
	left = opLeft;
	right = opRight;
}

//deleting AST
void deleteAST(ASTNode* &root) {
	if(root == nullptr)
		return;
	
	if(root->left == root->right) {
		deleteAST(root->left);
		root->right = nullptr;
		return;
	}
	
	deleteAST(root->left);
	deleteAST(root->right);
	
	delete root;
	
	root = nullptr;
}
#endif