#include <iostream>
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "interpreter.h"
#include "debugFuncs.h"
#include "error.h"

using namespace std;

int main(int argc, char *argv[]) {
	//only accept one argument, or "-debug" option
	bool isDebugOn = false;
	if(argc > 1) {
		string optionalArg = argv[1];
		if(optionalArg != "-debug") {
			cout << "Math-Intrptr Error: invalid argument, only accepted arguments are \"-debug\" or no additional arguments." << endl;
			return 0;
		} else {
			isDebugOn = true;
		}
	}
	
	//startup message
	cout << "simple c++ Math Interpreter." << endl;
	cout << "Type any accepted expression to evaluate or type \"exit()\" to close the program." << endl;
	
	//main input loop
	bool continueInterpreter = true;
	Interpreter interpret;
	while(continueInterpreter) {
		//get user input
		string inputLine;
		cout << ">>> ";
		getline(cin, inputLine);
		
		if(inputLine != "exit()") {
			try {
				Lexer lexer(inputLine);
				vector<Token> tokens = lexer.tokenize();
				representTokenList(tokens, isDebugOn);
				
				Parser parse(tokens);
				parse.parseAndConstructAST();
				ASTNode* astRoot = parse.getAST();
				if(isDebugOn) {
					cout << "AST: ";
					representAST(astRoot);
					cout << endl;
				}
				
				interpret.initialize(astRoot);
				interpret.doEvaluation();
				
				deleteAST(astRoot);
			}
			
			catch(MathInterpreterError &e) {
				cout << e.what() << endl;
			}
		} else {
			//close the program when "exit()" is inputted
			continueInterpreter = false;
		}
	}
}