#ifndef AST_H
#define AST_H

#include <iostream>
using namespace std;

enum NodeType {N_Number, N_Variable, N_Assign, N_Add, N_Subtract, N_Multipy, N_Divide};

class ASTNode {
	public:
		ASTNode* left;
		ASTNode* right;
		
		NodeType type;
		string nodeValue;
		
		ASTNode(NodeType initNodeType);
		void init_NumberNode(string number);
		void init_VariableNode(string var);
		void init_AssignNode(ASTNode* var, ASTNode* expression);
		void init_AddNode(ASTNode* opLeft, ASTNode* opRight);
		void init_SubtractNode(ASTNode* opLeft, ASTNode* opRight);
		void init_MultiplyNode(ASTNode* opLeft, ASTNode* opRight);
		void init_DivideNode(ASTNode* opLeft, ASTNode* opRight);
};

void deleteAST(ASTNode* &root);
#endif