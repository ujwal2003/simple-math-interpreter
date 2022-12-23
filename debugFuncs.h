#ifndef DEBUGFUNCS_H
#define DEBUGFUNCS_H

#include <iostream>
#include <vector>

#include "tokens.h"

using namespace std;

string representToken(Token t);
void representTokenList(vector<Token> tokens, bool debugFlag);

#endif