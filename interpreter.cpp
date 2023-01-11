#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include <stack>
#include <map>
#include "ast.h"

#include "interpreter.h"

using namespace std;

void Interpreter::raiseError() {
	deleteAST(astRoot);
	throw MathInterpreterError();
}

void Interpreter::initialize(ASTNode* asbtractSyntaxTree) {
	astRoot = asbtractSyntaxTree;
}

void Interpreter::evaluate(ASTNode* tree) {
	//if null do nothing
	if(tree == nullptr) {
		return;
	}
	//number node
	else if(tree->type == N_Number) {
		double num = stod(tree->nodeValue);
		evaluationStack.push(num);
		return;
	}
	//variable node
	else if(tree->type == N_Variable) {
		string varName = tree->nodeValue;
		if(symbolTable.find(varName) != symbolTable.end()) {
			double num = symbolTable[varName];
			evaluationStack.push(num);
			return;
		} else {
			cout << "Error: '" << varName << "' is not defined.";
			raiseError();
		}
	}
	//unary operator node
	else if(tree->type == N_UnaryOp) {
		evaluate(tree->left);
		double res = (tree->nodeValue == "-") ? (-1 * evaluationStack.top()) : evaluationStack.top();
		evaluationStack.pop();
		evaluationStack.push(res);
		return;
	}
	//assignment node
	else if(tree->type == N_Assign) {
		string varName;
		if(tree->left->type == N_Variable)
			varName = tree->left->nodeValue;
		else {
			cout << "Invalid syntax, expected identifier prior to equals sign. ";
			raiseError();
		}
		
		evaluate(tree->right);
		double num = evaluationStack.top();
		evaluationStack.pop();
		
		symbolTable[varName] = num;
		return;
	}
	//add node
	else if(tree->type == N_Add) {
		evaluate(tree->left);
		double leftOp = evaluationStack.top();
		evaluationStack.pop();
		
		evaluate(tree->right);
		double rightOp = evaluationStack.top();
		evaluationStack.pop();
		
		double res = leftOp + rightOp;
		evaluationStack.push(res);
		return;
	}
	//subtract node
	else if(tree->type == N_Subtract) {
		evaluate(tree->left);
		double leftOp = evaluationStack.top();
		evaluationStack.pop();
		
		evaluate(tree->right);
		double rightOp = evaluationStack.top();
		evaluationStack.pop();
		
		double res = leftOp - rightOp;
		evaluationStack.push(res);
		return;
	}
	//multiply node
	else if(tree->type == N_Multipy) {
		evaluate(tree->left);
		double leftOp = evaluationStack.top();
		evaluationStack.pop();
		
		evaluate(tree->right);
		double rightOp = evaluationStack.top();
		evaluationStack.pop();
		
		double res = leftOp * rightOp;
		evaluationStack.push(res);
		return;
	}
	//divide node
	else if(tree->type == N_Divide) {
		evaluate(tree->left);
		double leftOp = evaluationStack.top();
		evaluationStack.pop();
		
		evaluate(tree->right);
		double rightOp = evaluationStack.top();
		evaluationStack.pop();

		double res = leftOp / rightOp;
		evaluationStack.push(res);
		return;
	}
	//error
	else {
		cout << "Undetermined node type for abstract syntax tree node." << endl << "Node is either unknown or cannot be evaluated";
		raiseError();
	}
}

void Interpreter::doEvaluation() {
	if(astRoot == nullptr) {
		cout << endl;
	} else {
		evaluate(astRoot);
	}
	
	if(!evaluationStack.empty())
		cout << evaluationStack.top() << endl;
	
	while(!evaluationStack.empty())
		evaluationStack.pop();
}
#endif