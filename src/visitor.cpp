#include <assert.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include "tree_nodes.hpp"

using namespace ast;

using std::map;
using std::pair;
using std::string;

string to_string(yytokentype value){
	static map<yytokentype, string> token_to_string = {
		{(yytokentype)ABS, "ABS"},
		{(yytokentype)AND, "&&"},
		{(yytokentype)ATN, "ATN"},
		{(yytokentype)BOOLEAN, "BOOLEAN"},
		{(yytokentype)CHAR, "CHAR"},
		{(yytokentype)COMMA, "COMMA"},
		{(yytokentype)COMMENT, "COMMENT"},
		{(yytokentype)COS, "COS"},
		{(yytokentype)DATA, "DATA"},
		{(yytokentype)DEF, "DEF"},
		{(yytokentype)DIFF, "!="},
		{(yytokentype)DIM, "DIM"},
		{(yytokentype)DIVIDE, "/"},
		{(yytokentype)END, "END"},
		{(yytokentype)ENDL, "ENDL"},
		{(yytokentype)EQUALS, "EQUALS"},
		{(yytokentype)EXP, "EXP"},
		{(yytokentype)EXPONENTIAL, "^"},
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
		{(yytokentype)LT, "<"},
		{(yytokentype)LTE, "<="},
		{(yytokentype)MINUS, "-"},
		{(yytokentype)MOD, "%"},
		{(yytokentype)NEXT, "NEXT"},
		{(yytokentype)NOT, "!"},
		{(yytokentype)OR, "||"},
		{(yytokentype)PLUS, "+"},
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
		{(yytokentype)TIMES, "*"},
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
map<int, pair<string, string>> calls; // (idx, (parameter_expr, function_name))
map<string, pair<string, expr*> > function_code; // (label, (parameter_name, code))
string buffer;

void visitor::visit(const position& node) const{ }

void visitor::visit(const token& node) const{ }

void visitor::visit(const program& node) const{
	string code, main;
	
	for(auto _stmt = node.stmts.cbegin(); _stmt != node.stmts.cend(); _stmt++){
		(*_stmt)->accept(*this);
		main += buffer;
	}
	
	std::ifstream in("src/activation_records.cpp");
	
	using std::cin;
	
	std::streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in.rdbuf());
	
	string line;
	do{
		std::getline(cin, line);
		code += line + "\n";
	} while(line.find("enum label") == string::npos);

	for(string label : labels){
		code += label + ",\n";
	}
	
	do{
		std::getline(cin, line);
		code += line + "\n";
	} while(line.find("int main") == string::npos);
	
	code += main;
	
	code += "transfer:\n";
	code += "switch(next_label){\n";
	for(string& label : labels){
		code += "\tcase " + label + ":\n";
		code += "\t\tgoto " + label + ";\n";
		code += "\t\tbreak;\n";
	}
	code += "}";

	while(std::getline(std::cin, line)){
		code += line + "\n";
	}
	
	cin.rdbuf(cinbuf);
	
	buffer = code;
	std::cout << buffer;
}

void visitor::visit(const end_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	code += "return 0;\n";
	buffer = code;
}

void visitor::visit(const let_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	const auto& var = node.var;
	const auto& val = node.val;
	string code = label + ":\n";
	if(var->idx1 != nullptr){		
		solve_expr(this, *var->idx1, label + "_idx1", "idx1");
		code += buffer;
		code += verify_index("idx1");
		buffer.clear();
	}
	else{
		code += create_default_index("idx1");
	}
	if(var->idx2 != nullptr){		
		solve_expr(this, *var->idx2, label + "_idx2", "idx2");
		code += buffer;
		code += verify_index("idx2");
		buffer.clear();
	}
	else{
		code += create_default_index("idx2");
	}
	solve_expr(this, *val, label + "_val", "target");
	code += buffer;
	code += "let(\"" + *var->name + "\", idx1, idx2, target);\n";
	buffer = code;
}

void visitor::visit(const print_stmt& node) const{ }

void visitor::visit(const read_stmt& node) const{ }

void visitor::visit(const input_stmt& node) const{ }

void visitor::visit(const data_stmt& node) const{ }

void visitor::visit(const goto_stmt& node) const{ }

void visitor::visit(const if_stmt& node) const{ }

void visitor::visit(const gosub_stmt& node) const{ }

void visitor::visit(const return_stmt& node) const{ }

void visitor::visit(const def_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string label_impl = *node.name + "_" + to_string(node.line);
	labels.push_back(label_impl);
	function_code[label_impl] = std::make_pair(*node.param, node.target);
	string code = label + ":\n";
	code += "update_def(\"" + *node.name + "\", " + label_impl + ");\n";
	buffer = code;
}

void visitor::visit(const dim_stmt& node) const{ }

void visitor::visit(const next_stmt& node) const{ }

void visitor::visit(const for_stmt& node) const{ }

void visitor::visit(const stop_stmt& node) const{ }

void visitor::visit(const binary_expr& node) const{
	node.left->accept(*this);
	string code = buffer;
	code += " " + to_string(node.tok.id) + " ";
	node.right->accept(*this);
	buffer = code + buffer;
}

void visitor::visit(const unary_expr& node) const{ }

void visitor::visit(const function_expr& node) const{
	string code;
	node.param->accept(*this);
	if(node.name->operator[](0) == 'F' || node.name->operator[](0) == 'f'){
		int id = (int) calls.size();
		calls[id] = std::make_pair(buffer, *node.name);
		code = string("t") + to_string(id);
	}
	else{
		string name = "";
		for(char c : *node.name){
			name += (char)tolower(c);
		}
		code = name + "(" + buffer + ")";
	}
	buffer = code;
}

void visitor::visit(const variable& node) const{ }

template<class T> void visitor::visit(const literal_expr<T>& node) const{
	buffer = to_string(node.value);
}

template void visitor::visit<string*>(const literal_expr<string*>& node) const;
template void visitor::visit<int>(const literal_expr<int>& node) const;
template void visitor::visit<bool>(const literal_expr<bool>& node) const;

void solve_expr(const visitor* vis, const expr& exp, string label, string target){
	string code = "value " + target + ";\n";
	code += "{\n";
	exp.accept(*vis);
	for(auto it = calls.cbegin(); it != calls.cend(); it++){
		for(auto it2 = calls.cbegin(); it2 != it; it2++){
			code += "push_value(t" + to_string(it2->first) + ");\n";
		}
		auto& t = *it;
		code += "next_label = get_def(\"" + t.second.second + "\");\n";
		string new_label = label + "_t" + to_string(t.first);
		labels.push_back(new_label);
		code += "push_function_call(" + new_label + ");\n";
		code += "push_parameter(" + t.second.first + ");\n";
		code += "goto transfer;\n";
		code += new_label + ":\n";		
		code += "value t" + to_string(t.first) + " = " + "get_return_value();\n";
		for(auto it2 = calls.cbegin(); it2 != it; it2++){
			code += "t" + to_string(it2->first) + " = pop_value();\n";
		}
	}
	calls.clear();
	code += target + " = " + buffer + ";\n";
	code += "}\n";
	buffer = code;
}

string verify_index(string idx){
	string code = "verify_index(" + idx + ");\n";
	return code;
}

string create_default_index(string idx){
	return "int " + idx + " = -1;\n";
}

