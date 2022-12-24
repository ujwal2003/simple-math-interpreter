#ifndef ERROR_H
#define ERROR_H

#include <iostream>

using namespace std;

struct MathInterpreterError : public exception {
	const char * what () throw () {
		return "Error encountered.";
	}
};

#endif