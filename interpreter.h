#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <stack>
#include <map>
#include "ast.h"
#include "error.h"

using namespace std;

class Interpreter {
	private:
		stack<double> evaluationStack;
		map<string, double> symbolTable;
		
		ASTNode* astRoot;
		void raiseError();
		
	public:
		void initialize(ASTNode* asbtractSyntaxTree);
		void evaluate(ASTNode* tree);
		void doEvaluation();
};

#endif