#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	//only accept one argument
	if(argc > 1) {
		cout << "Math-Intrptr : additional arguments are not accepted." << endl;
		return 0;
	}
	
	//startup message
	cout << "simple c++ Math Interpreter." << endl;
	cout << "Type 'exit()' and hit enter to close the program." << endl;
	
	//main input loop
	bool continueInterpreter = true;
	while(continueInterpreter) {
		//get user input
		string inputLine;
		cout << ">>> ";
		cin >> inputLine;
		
		if(inputLine != "exit()") {
			cout << inputLine << endl; //temporary
		} else {
			//close the program when "exit()" is inputted
			continueInterpreter = false;
		}
	}
}