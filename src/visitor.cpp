#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>

#include "tree_nodes.hpp"

using namespace ast;

using std::cout;
using std::endl;
using std::map;
using std::string;

string to_string(yytokentype value){
	static map<yytokentype, string> token_to_string = {
		{(yytokentype)ABS, "ABS"},
		{(yytokentype)AND, "AND"},
		{(yytokentype)ATN, "ATN"},
		{(yytokentype)BOOLEAN, "BOOLEAN"},
		{(yytokentype)CHAR, "CHAR"},
		{(yytokentype)COMMA, "COMMA"},
		{(yytokentype)COMMENT, "COMMENT"},
		{(yytokentype)COS, "COS"},
		{(yytokentype)DATA, "DATA"},
		{(yytokentype)DEF, "DEF"},
		{(yytokentype)DIFF, "DIFF"},
		{(yytokentype)DIM, "DIM"},
		{(yytokentype)DIVIDE, "DIVIDE"},
		{(yytokentype)END, "END"},
		{(yytokentype)ENDL, "ENDL"},
		{(yytokentype)EQUALS, "EQUALS"},
		{(yytokentype)EXP, "EXP"},
		{(yytokentype)EXPONENTIAL, "EXPONENTIAL"},
		{(yytokentype)FLOAT, "FLOAT"},
		{(yytokentype)FOR, "FOR"},
		{(yytokentype)FUNCTION, "FUNCTION"},
		{(yytokentype)GOSUB, "GOSUB"},
		{(yytokentype)GOTO, "GOTO"},
		{(yytokentype)GT, "GT"},
		{(yytokentype)GTE, "GTE"},
		{(yytokentype)IF, "IF"},
		{(yytokentype)INPUT, "INPUT"},
		{(yytokentype)INT, "INT"},
		{(yytokentype)INTEGER, "INTEGER"},
		{(yytokentype)LET, "LET"},
		{(yytokentype)LEXEOF, "LEXEOF"},
		{(yytokentype)LEXERROR, "LEXERROR"},
		{(yytokentype)LOG, "LOG"},
		{(yytokentype)LPAREN, "LPAREN"},
		{(yytokentype)LT, "LT"},
		{(yytokentype)LTE, "LTE"},
		{(yytokentype)MINUS, "MINUS"},
		{(yytokentype)MOD, "MOD"},
		{(yytokentype)NEXT, "NEXT"},
		{(yytokentype)NOT, "NOT"},
		{(yytokentype)OR, "OR"},
		{(yytokentype)PLUS, "PLUS"},
		{(yytokentype)PRINT, "PRINT"},
		{(yytokentype)READ, "READ"},
		{(yytokentype)RETURN, "RETURN"},
		{(yytokentype)RND, "RND"},
		{(yytokentype)RPAREN, "RPAREN"},
		{(yytokentype)SEMICOLON, "SEMICOLON"},
		{(yytokentype)SIN, "SIN"},
		{(yytokentype)SQR, "SQR"},
		{(yytokentype)STEP, "STEP"},
		{(yytokentype)STOP, "STOP"},
		{(yytokentype)STRING, "STRING"},
		{(yytokentype)TAN, "TAN"},
		{(yytokentype)THEN, "THEN"},
		{(yytokentype)TIMES, "TIMES"},
		{(yytokentype)TO, "TO"},
		{(yytokentype)VARIABLE, "VARIABLE"},
		{(yytokentype)WHITE, "WHITE"},
	};
	return token_to_string[value];
}

inline string to_string(int value){
	return std::to_string(value);
}

inline string to_string(bool value){
	return (value ? "true" : "false");
}

inline string to_string(string* value){
	return *value;
}

vector<string> labels;
map<int, string> calls;

void visitor::visit(const position& node) const{ }

void visitor::visit(const token& node) const{ }

void visitor::visit(const program& node) const{ }

void visitor::visit(const end_stmt& node) const{ }

void visitor::visit(const let_stmt& node) const{ }

void visitor::visit(const print_stmt& node) const{ }

void visitor::visit(const read_stmt& node) const{ }

void visitor::visit(const input_stmt& node) const{ }

void visitor::visit(const data_stmt& node) const{ }

void visitor::visit(const goto_stmt& node) const{ }

void visitor::visit(const if_stmt& node) const{ }

void visitor::visit(const gosub_stmt& node) const{ }

void visitor::visit(const return_stmt& node) const{ }

void visitor::visit(const def_stmt& node) const{ }

void visitor::visit(const dim_stmt& node) const{ }

void visitor::visit(const next_stmt& node) const{ }

void visitor::visit(const for_stmt& node) const{ }

void visitor::visit(const stop_stmt& node) const{ }

void visitor::visit(const binary_expr& node) const{ }

void visitor::visit(const unary_expr& node) const{ }

void visitor::visit(const function_expr& node) const{ }

void visitor::visit(const variable& node) const{ }

template<class T> void visitor::visit(const literal_expr<T>& node) const{ }

template void visitor::visit<string*>(const literal_expr<string*>& node) const;
template void visitor::visit<int>(const literal_expr<int>& node) const;
template void visitor::visit<bool>(const literal_expr<bool>& node) const;

