#include <iostream>
#include "tokens.h"
#include "lexer.h"
#include "debugFuncs.h"

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
	while(continueInterpreter) {
		//get user input
		string inputLine;
		cout << ">>> ";
		getline(cin, inputLine);
		
		if(inputLine != "exit()") {
			Lexer lexer(inputLine);
			vector<Token> tokens = lexer.tokenize();
			representTokenList(tokens, isDebugOn);
		} else {
			//TODO: add "help()" message
			//close the program when "exit()" is inputted
			continueInterpreter = false;
		}
	}
}