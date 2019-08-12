#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <map>

#include "parser.h"
#include "symbol.h"
#include "token.h"

using std::pair;
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::map;

struct Node {
	Node* father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
	
	Node(symbol s, string l = "") : sym(s), lexeme(l) { }
};

map<symbol, string> symbol_to_string = {
	{T_ABS, "ABS"},
	{T_AND, "AND"},
	{T_ATN, "ATN"},
	{T_BOOLEAN, "BOOLEAN"},
	{T_CHAR, "CHAR"},
	{T_COMMA, ","},
	{T_COMMENT, "COMMENT"},
	{T_COS, "COS"},
	{T_DATA, "DATA"},
	{T_DEF, "DEF"},
	{T_DIFF, "<>"},
	{T_DIM, "DIM"},
	{T_DIVIDE, "/"},
	{T_END, "END"},
	{T_ENDL, "ENDL"},
	{T_EQUALS, "="},
	{T_EXP, "EXP"},
	{T_EXPONENTIAL, "^"},
	{T_FLOAT, "FLOAT"},
	{T_FOR, "FOR"},
	{T_FUNCTION, "FUNCTION"},
	{T_GOSUB, "GOSUB"},
	{T_GOTO, "GOTO"},
	{T_GT, ">"},
	{T_GTE, ">="},
	{T_IF, "IF"},
	{T_INPUT, "INPUT"},
	{T_INT, "INT"},
	{T_INTEGER, "INTEGER"},
	{T_LET, "LET"},
	{T_LEXEOF, "EOF"},
	{T_LOG, "LOG"},
	{T_LPAREN, "("},
	{T_LT, "<"},
	{T_LTE, "<="},
	{T_MINUS, "-"},
	{T_MOD, "%"},
	{T_NEXT, "NEXT"},
	{T_NOT, "NOT"},
	{T_OR, "OR"},
	{T_PLUS, "+"},
	{T_PRINT, "PRINT"},
	{T_READ, "READ"},
	{T_RETURN, "RETURN"},
	{T_RND, "RND"},
	{T_RPAREN, ")"},
	{T_SEMICOLON, ";"},
	{T_SIN, "SIN"},
	{T_SQR, "SQR"},
	{T_STEP, "STEP"},
	{T_STOP, "STOP"},
	{T_STRING, "STRING"},
	{T_TAN, "TAN"},
	{T_THEN, "THEN"},
	{T_TIMES, "*"},
	{T_TO, "TO"},
	{T_VARIABLE, "VARIABLE"},
	{T_WHITE, "WHITE"},
	{PROGRAM, "<program>"},
	{ANDEXP2, "<and_exp2>"},
	{EXPOEXP2, "<expo_exp2>"},
	{EXPR_LIST2, "<expr_list2>"},
	{FORINIT, "<forinit>"},
	{FORSTEP, "<forstep>"},
	{NUM_LIST2, "<num_list2>"},
	{FORBLOCK, "<forblock>"},
	{NUMERATED_FOR, "<numerated_for>"},
	{FORNEXT, "<fornext>"},
	{FORBODY, "<forbody>"},
	{NUMERATED_MAIN, "<numerated_main>"},
	{NUMERATED_BLOCK, "<numerated_block>"},
	{BLOCKS, "<blocks>"},
	{OREXP, "<or_exp>"},
	{ANDEXP, "<and_exp>"},
	{OREXP2, "<or_exp2>"},
	{EXPOEXP, "<expo_exp>"},
	{PRODEXP2, "<prod_exp2>"},
	{RELEXP, "<rel_exp>"},
	{RELEXP2, "<rel_exp2>"},
	{RELOP, "<rel_op>"},
	{STMT, "<stmt>"},
	{EXPR_LIST, "<expr_list>"},
	{NUM_LIST, "<num_list>"},
	{SUMEXP, "<sum_exp>"},
	{PRODEXP, "<prod_exp>"},
	{SUMEXP2, "<sum_exp2>"},
	{UNARYEXP, "<unary_exp>"},
	{NATIVE_FUNCTION, "<native_function>"},
	{UNARYOP, "<unary_op>"},
	{EXPR, "<expr>"},
	{IDX2, "<idx2>"},
	{VARIABLE2, "<variable2>"},
	{VARIABLE_LIST, "<variable_list>"},
	{VAR, "<var>"},
	{VARIABLE_LIST2, "<variable_list2>"},
	{EMPTY_LINES, "<empty_lines>"},
};

struct Node{
	Node *father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
	Node(symbol s, string l) : 
		sym(s), lexeme(l)
	{
		children = vector<Node*>();
	}
};

string convertToLatex(string text){
	string ret = "";
	for(int i = 0; i < text.size(); i++){
		if(text[i] == '_'){
			ret += "\\textunderscore ";
		}
		else if(text[i] == '<'){
			ret += "$<$";
		}
		else if(text[i] == '>'){
			ret += "$>$";
		}
		else{
			ret += text[i];
		}
	}
	return ret;
}

string createTikzTree(Node *root, int depth=0){
	if(root == NULL) return "";
	string ret = "";
	if(depth == 0){
		string text = (root->lexeme == "" ? symbol_to_string[root->sym] : root->lexeme);
		ret += "\\node{" + convertToLatex(text) + "}\n";
		for(auto child : root->children){
			ret += "child {\n";
			ret += createTikzTree(child, depth+1);
			ret += "}\n";
		}
	}
	else{
		for(int i = 0; i < depth; i++) ret += "\t";
		string text = (root->lexeme == "" ? symbol_to_string[root->sym] : root->lexeme);
		ret += "node{" + convertToLatex(text) + "}\n";
		for(auto child : root->children){
			for(int i = 0; i < depth; i++) ret += "\t";
			ret += "child {\n";
			ret += createTikzTree(child, depth+1);
			for(int i = 0; i < depth; i++) ret += "\t";
			ret += "}\n";
		}
	}
	return ret;
}

int main(){
	/*
		THE 'ROOT' NODE BELOW REPRESENTS THE AST OF THE FOLLOWING PROGRAM:
			"1 END\n"
	 */

	Node *root = new Node(PROGRAM, "");
	root->children.push_back(new Node(BLOCKS, ""));
	root->children[0]->children.push_back(new Node(T_INTEGER, "1"));
	root->children[0]->children.push_back(new Node(NUMERATED_MAIN, ""));
	root->children[0]->children[1]->children.push_back(new Node(T_END, ""));
	root->children[0]->children[1]->children.push_back(new Node(T_ENDL, ""));
	root->children[0]->children[1]->children.push_back(new Node(EMPTY_LINES, ""));
	root->children[0]->children[1]->children.push_back(new Node(T_LEXEOF, ""));

	cout << createTikzTree(root) << "\n";
}
