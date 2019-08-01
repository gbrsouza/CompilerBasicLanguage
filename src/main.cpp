#include <iostream>
#include <map>
#include <string>

#include "token.h"

using std::cout;
using std::endl;
using std::string;
using std::map;

map<token, string> trad = {
	{ABS, "ABS"},
	{AND, "AND"},
	{ATN, "ATN"},
	{BOOL, "BOOL"},
	{CHAR, "CHAR"},
	{COMMA, "COMMA"},
	{COMMENT, "COMMENT"},
	{COS, "COS"},
	{DATA, "DATA"},
	{DEF, "DEF"},
	{DIFF, "DIFF"},
	{DIM, "DIM"},
	{DIVIDE, "DIVIDE"},
	{END, "END"},
	{ENDL, "ENDL"},
	{EQUALS, "EQUALS"},
	{EXP, "EXP"},
	{EXPONENTIAL, "EXPONENTIAL"},
	{FLOAT, "FLOAT"},
	{FOR, "FOR"},
	{FUNCTION, "FUNCTION"},
	{GOSUB, "GOSUB"},
	{GOTO, "GOTO"},
	{GT, "GT"},
	{GTE, "GTE"},
	{IF, "IF"},
	{INPUT, "INPUT"},
	{INT, "INT"},
	{INTEGER, "INTEGER"},
	{LET, "LET"},
	{LEXEOF, "LEXEOF"},
	{LEXERROR, "LEXERROR"},
	{LOG, "LOG"},
	{LPAREN, "LPAREN"},
	{LT, "LT"},
	{LTE, "LTE"},
	{MINUS, "MINUS"},
	{MOD, "MOD"},
	{NEXT, "NEXT"},
	{NOT, "NOT"},
	{OR, "OR"},
	{PLUS, "PLUS"},
	{PRINT, "PRINT"},
	{READ, "READ"},
	{RETURN, "RETURN"},
	{RND, "RND"},
	{RPAREN, "RPAREN"},
	{SEMICOLON, "SEMICOLON"},
	{SIN, "SIN"},
	{SQR, "SQR"},
	{STEP, "STEP"},
	{STOP, "STOP"},
	{STRING, "STRING"},
	{TAN, "TAN"},
	{THEN, "THEN"},
	{TIMES, "TIMES"},
	{TO, "TO"},
	{VARIABLE, "VARIABLE"},
	{WHITE, "WHITE"},
};

int main(){
	token cur;
	while((cur = nextToken()) != LEXEOF) {
		if(cur == WHITE){
			continue;
		}
		
		cout << "TokenId: " << trad[cur] << endl;
		cout << "Line: " << line << endl;
		cout << "Column: " << column << endl;
		if(cur == FLOAT || cur == INTEGER || cur == FUNCTION || cur == VARIABLE || 
		   cur == STRING || cur == CHAR || cur == LEXERROR){
			cout << "Text: " << text << endl;
		}
		
		cout << endl;
	}
}
