#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>

#include "tree_nodes.h"
#include "visitor.h"

using namespace ast;

using std::cout;
using std::map;
using std::string;

map<yytokentype, string> token_to_string = {
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

void visitor::visit(const position& node) const{
	cout << "{\"class\" : \"POSITION\","
		<< "\"line\" : "
		<< node.line
		<< ",\"column\" : "
		<< node.column
		<< "}";
}

void visitor::visit(const token& node) const{
	cout << "{"
		<< "\"class\" : \"TOKEN\","
		<< "\"id\" : \"" << token_to_string[node.id]
		<< "\",\"pos\" :";
	node.pos.accept(*this);
	cout << "}";
}

void visitor::visit(const program& node) const{
	cout << "{"
		<< "\"class\" : \"PROGRAM\","
		<< "\"stmts\" : [";
	for(auto _stmt = node.stmts.cbegin(); _stmt != node.stmts.cend(); _stmt++){
		if(_stmt != node.stmts.cbegin()){
			cout << ",";
		}
		(*_stmt)->accept(*this);
	}
	cout << "]";
	cout << "}";
	cout << std::endl;
}

void visitor::visit(const end_stmt& node) const{
	cout << "{"
		<< "\"class\" : \"END_STMT\","
		<< "\"tok\" :";
	node.tok.accept(*this);
	cout << ",\"line\" : "
		<< node.line
		<< "}";
}

void visitor::visit(const let_stmt& node) const{
	cout << "{"
		<< "\"class\" : \"LET_STMT\","
		<< "\"tok\" :";
	node.tok.accept(*this);
	cout << ",\"var\" : ";
	node.var->accept(*this);
	cout << ",\"val\" : ";
	node.val->accept(*this);
	cout << "}";
}

void visitor::visit(const print_stmt& node) const{ }

void visitor::visit(const read_stmt& node) const{ }

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

void visitor::visit(const binary_expr& node) const{
	cout << "{"
		<< "\"class\" : \"BINARY_EXPR\",";
	cout << "\"left\" : ";
	node.left->accept(*this);
	cout << ",\"tok\" :";
	node.tok.accept(*this);
	cout << ",\"right\" : ";
	node.right->accept(*this);
	cout << "}";
}

void visitor::visit(const unary_expr& node) const{ }

void visitor::visit(const function_expr& node) const{ }

void visitor::visit(const variable& node) const{
	cout << "{"
		<< "\"class\" : \"VARIABLE\",";
	cout << "\"name\" : \"" << *node.name << "\"";
	if(node.idx1 != nullptr){
		cout << ",\"idx1\" : ";
		node.idx1->accept(*this);
	}
	if(node.idx2 != nullptr){
		cout << ",\"idx2\" : ";
		node.idx2->accept(*this);
	}
	cout << "}";
}

string to_string(int value){
	return std::to_string(value);
}

string to_string(bool value){
	return (value ? "true" : "false");
}

string to_string(string* value){
	return *value;
}

template<class T> void visitor::visit(const literal_expr<T>& node) const{
	cout << "{"
		<< "\"class\" : \"LITERAL_EXPR\",";
	cout << "\"value\" : \"" << to_string(node.value) << "\"";
	cout << "}";
}

template void visitor::visit<string*>(const literal_expr<string*>& node) const;
template void visitor::visit<int>(const literal_expr<int>& node) const;
template void visitor::visit<bool>(const literal_expr<bool>& node) const;

