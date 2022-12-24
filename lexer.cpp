#ifndef LEXER_CPP
#define LEXER_CPP

#include <iostream>
#include <vector>

#include "tokens.h"
#include "lexer.h"

using namespace std;

//advance to next character
void Lexer::advanceChar() {
	lineIdx += 1;
	if(lineIdx < line.size() && lineIdx >= 0) {
		currChar = line[lineIdx];
	} else {
		currChar = '\0';
	}
}

//constructor
Lexer::Lexer(string inputLine) {
	lineIdx = -1;
	currChar = '\0';
	line = inputLine;
	advanceChar();
}

//take input and create list of tokens
vector<Token> Lexer::tokenize() {
	vector<Token> tokens;
	while(currChar != '\0') {
		//empty space
		if(isspace(currChar)) {
			advanceChar();
		}
		//number
		else if(currChar == '.' || isdigit(currChar)) {
			tokens.push_back(createNumber());
		}
		//variable
		else if(isalpha(currChar)) {
			tokens.push_back(createVariable());
		}
		//operators and parentheses
		else if(currChar == '=' || currChar == '(' || currChar == ')' 
				|| currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/') {
			tokens.push_back(getOperatorOrParen());
			advanceChar();
		}
		//error
		else {
			cout << "invalid character error ---> " << currChar << endl;
			tokens.clear();
			return tokens;
		}
	}
	
	return tokens;
}

//create a number token
Token Lexer::createNumber() {
	int decPointCounter = 0;
	string number = "";
	number += currChar;
	advanceChar();
	
	while(currChar != '\0' && (currChar == '.' || isdigit(currChar))) {
		if(currChar == '.') {
			decPointCounter += 1;
			if(decPointCounter > 1){break;}
		}
		
		number += currChar;
		advanceChar();
	}
	
	if(number[0] == '.')
		number = '0' + number;
	
	if(number[number.size()-1] == '.')
		number += '0';
	
	return Token(T_Number, number);
}

//create operator or separator tokens
Token Lexer::getOperatorOrParen() {
	switch(currChar) {
		case '=': return Token(T_Equal);
		case '(': return Token(T_OpenParen);
		case ')': return Token(T_ClosedParen);
		case '+': return Token(T_Plus);
		case '-': return Token(T_Minus);
		case '*': return Token(T_Multiply);
		case '/': return Token(T_Divide);
		default: return Token(T_UNKNOWN);
	}
}

//create variable token
Token Lexer::createVariable() {
	string var = "";
	var += currChar;
	advanceChar();
	
	while(currChar != '\0' && isalnum(currChar)) {
		var += currChar;
		advanceChar();
	}
	
	return Token(T_Variable, var);
}
#endif