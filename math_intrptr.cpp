#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	//only accept one argument
	if(argc > 1) {
		cout << "Math-Intrptr : additional arguments are not accepted." << endl;
		return 0;
	}
	
	//main loop
	bool continueInterpreter = true;
	while(continueInterpreter) {
		string inputLine;
		cout << ">>> ";
		cin >> inputLine;
		
		if(inputLine != "exit()") {
			cout << inputLine << endl; //temporary
		} else {
			continueInterpreter = false;
		}
	}
}