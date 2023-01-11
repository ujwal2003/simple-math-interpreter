#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include <stack>
#include <map>
#include "ast.h"

#include "interpreter.h"

using namespace std;

void Interpreter::raiseError() {
	deleteAST(tree);
	throw MathInterpreterError();
}

Interpreter::Interpreter(ASTNode* abstractSyntaxTree) {
	tree = abstractSyntaxTree;
}

void Interpreter::evaluate() {
	return;
}
#endif