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
		{(yytokentype)EQUALS, "=="},
		{(yytokentype)EXP, "EXP"},
		{(yytokentype)EXPONENTIAL, "^"},
		{(yytokentype)FLOAT, "FLOAT"},
		{(yytokentype)FOR, "FOR"},
		{(yytokentype)FUNCTION, "FUNCTION"},
		{(yytokentype)GOSUB, "GOSUB"},
		{(yytokentype)GOTO, "GOTO"},
		{(yytokentype)GT, ">"},
		{(yytokentype)GTE, ">="},
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
string current_parameter;

void get_var_idxs(const visitor* vis, string label, const expr* idx1, const expr* idx2);
void solve_expr(const visitor* vis, const expr& exp, string label, string target);
string verify_index(string idx);
string create_default_index(string idx);

void visitor::visit(const position& ) const{ }

void visitor::visit(const token& ) const{ }

void visitor::visit(const program& node) const{
	string code, main;
	
	auto add_to_main = [&](const string s){
		static int ident = 1;
		static bool init_of_line = true;
		
		for(char c : s){
			if(init_of_line){
				if(c != '}'){
					main += '\t';
				}
				init_of_line = false;
			}
			main += c;
			if(c == '\n'){
				for(int j = 0; j < ident - 1; j++){
					main += '\t';
				}
				if(ident > 0){
					init_of_line = true;
				}
			}
			if(c == '{'){
				ident++;
			}
			if(c == '}'){
				ident--;
			}
		}
	};
	
	for(auto _stmt = node.stmts.cbegin(); _stmt != node.stmts.cend(); _stmt++){
		(*_stmt)->accept(*this);
		add_to_main(buffer);
	}
	
	labels.push_back("invalid");
	
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
	
	for(auto& func : function_code){
		const string& func_label = func.first;
		string& param_name = func.second.first;
		expr& e = *func.second.second;
		add_to_main(func_label + ": {\n");
		add_to_main("parameter = pop_parameter();\n");
		current_parameter = param_name;
		solve_expr(this, e, func_label + "_impl", "result");
		current_parameter = "";
		add_to_main(buffer);
		add_to_main("next_label = pop_function_call();\n");
		add_to_main("set_return_value(result);\n");
		add_to_main("goto transfer;\n");
		add_to_main("}\n");
	}
	
	add_to_main("transfer:\n");
	add_to_main("switch(next_label){\n");
	for(string& label : labels){
		add_to_main("case " + label + ":\n");
		add_to_main("\tgoto " + label + ";\n");
		add_to_main("\tbreak;\n");
	}
	add_to_main("}\n");
	add_to_main("invalid: undefined_control_flow();\n");
	
	code += main;
	
	while(std::getline(std::cin, line)){
		code += line + "\n";
	}
	
	cin.rdbuf(cinbuf);
	
	buffer = code;
	
	std::cout << buffer;
	
	labels.clear();
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
	string code = label + ": {\n";
	get_var_idxs(this, label, var->idx1, var->idx2);
	code += buffer;
	code += "push_value(idx1);\n";
	code += "push_value(idx2);\n";
	solve_expr(this, *val, label + "_val", "target");
	code += buffer;
	code += "idx2 = pop_value();\n";
	code += "idx1 = pop_value();\n";
	code += "let(\"" + *var->name + "\", idx1.content._int, idx2.content._int, target);\n";
	code += "}\n";
	buffer = code;
}

void visitor::visit(const print_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	int val_count = 0;
	for(print_expr& cur_expr : *node.values){
		code += "{\n";
		solve_expr(this, *cur_expr.first, label + "_" + to_string(val_count++), "target");
		code += buffer;
		code += "print(target, " + to_string(cur_expr.second) + ");\n";
		code += "}\n";
	}
	buffer = code;
}

void visitor::visit(const read_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	int var_count = 0;
	for(variable* var : *node.var_list){
		code += "{\n";
		get_var_idxs(this, label + "_" + to_string(var_count++), var->idx1, var->idx2);
		code += buffer;
		code += "read(\"" + *var->name + "\", idx1.content._int, idx2.content._int);\n";
		code += "}\n";
	}
	buffer = code;
}

void visitor::visit(const input_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	int var_count = 0;
	for(variable* var : *node.var_list){
		code += "{\n";
		get_var_idxs(this, label + "_" + to_string(var_count++), var->idx1, var->idx2);
		code += buffer;
		code += "input(\"" + *var->name + "\", idx1.content._int, idx2.content._int);\n";
		code += "}\n";
	}
	buffer = code;
}

void visitor::visit(const data_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	for(expr* numeric_value : *node.num_list){
		numeric_value->accept(*this);
		code += "data(" + buffer + ");\n";
	}
	buffer = code;
}

void visitor::visit(const goto_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	string target_label = "l" + to_string(node.target_line);
	code += "goto " + target_label + ";\n";
	buffer = code;
}

void visitor::visit(const if_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ": {\n";
	string target_label = "l" + to_string(node.target_line);
	solve_expr(this, *node.condition, label + "_condition", "condition");
	code += buffer;
	code += "if(condition.content._bool) goto " + target_label + ";\n";
	code += "}\n";
	buffer = code;
}

void visitor::visit(const gosub_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string return_label = label + "_ret";
	labels.push_back(return_label);
	string code = label + ":\n";
	string target_label = "l" + to_string(node.target_line);
	code += "push_function_call(" + return_label + ");\n";
	code += "goto " + target_label + ";\n";
	code += return_label + ":\n";
	buffer = code;
}

void visitor::visit(const return_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	code += "next_label = pop_function_call();\n";
	code += "goto transfer;\n";
	buffer = code;
}

void visitor::visit(const def_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string label_impl = *node.name + "_" + to_string(node.line);
	labels.push_back(label_impl);
	function_code[label_impl] = std::make_pair(*node.param, node.target);
	string code = label + ":\n";
	code += "def(\"" + *node.name + "\", " + label_impl + ");\n";
	buffer = code;
}

void visitor::visit(const dim_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	const auto& var = node.var;
	string code = label + ": {\n";
	get_var_idxs(this, label, var->idx1, var->idx2);
	code += buffer;
	code += "dim(\"" + *var->name + "\", idx1.content._int , idx2.content._int);\n";
	code += "}\n";
	buffer = code;
}

void visitor::visit(const next_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string label_exit_loop = label + "_exit_loop";
	labels.push_back(label_exit_loop);
	const string& var_name = *node.var->name;
	string code = label + ":\n";
	code += "verify_loop_variable(\"" + var_name + "\");\n";
	code += "pop_loop_variable();\n";
	code += "next_label = pop_loop_label();\n";
	code += "push_next_stmt_label(" + label_exit_loop + ");\n";
	code += "goto transfer;\n";
	code += label_exit_loop + ":\n";
	buffer = code;
}

void visitor::visit(const for_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ": {\n";
	string label_condition = label + "_condition";
	labels.push_back(label_condition);
	string label_step = label + "_step";
	labels.push_back(label_step);
	
	const string& var_name = *node.var->name;
	const expr* initial_value = node.initial_value;
	const expr* condition = node.condition;
	const expr* step = node.step;
	
	solve_expr(this, *initial_value, label + "_initial", "initial");
	code += buffer;
	code += "let(\"" + var_name + "\", -1, -1, initial);\n";
	code += "}\n";
	code += "{\n";
	solve_expr(this, *condition, label + "_last", "last");
	code += buffer;
	code += "push_loop_last_value(last);\n";
	code += "}\n";
	code += "{\n";
	if(step == nullptr){
		code += "value step;\n";
		code += "step = to_value(1);\n";
	}
	else{
		solve_expr(this, *step, label + "_step", "step");
		code += buffer;
	}
	code += "push_loop_step(step);\n";
	code += "push_next_stmt_label(invalid);\n";
	code += "}\n";
	code += "goto " + label_condition + ";\n";
	code += label_step + ": {\n";
	code += "label exit_loop_label = pop_next_stmt_label();\n";
	code += "value step = pop_loop_step();\n";
	node.var->accept(*this);
	code += "let(\"" + var_name + "\", -1, -1, " + buffer + " + step);\n";
	code += "push_loop_step(step);\n";
	code += "push_next_stmt_label(exit_loop_label);\n";
	code += "}\n";
	code += label_condition + ": {\n";
	code += "next_label = pop_next_stmt_label();\n";
	code += "value step = pop_loop_step();\n";
	code += "value last = pop_loop_last_value();\n";
	code += "bool condition_1 = is_negative(step) && (" + buffer + " >= last).content._bool;\n";
	code += "bool condition_2 = !is_negative(step) && (" + buffer + " <= last).content._bool;\n";
	code += "bool condition = condition_1 || condition_2;\n";
	code += "if(!condition) goto transfer;\n";
	code += "push_loop_last_value(last);\n";
	code += "push_loop_step(step);\n";
	code += "push_loop_label(" + label_step + ");\n";
	code += "push_loop_variable(\"" + var_name + "\");\n";
	code += "}\n";
	buffer = code;
}

void visitor::visit(const stop_stmt& node) const{
	string label = "l" + to_string(node.line);
	labels.push_back(label);
	string code = label + ":\n";
	code += "return 0;\n";
	buffer = code;
}

void visitor::visit(const binary_expr& node) const{
	node.left->accept(*this);
	string code = buffer;
	code += " " + to_string(node.tok.id) + " ";
	node.right->accept(*this);
	buffer = "(" + code + buffer + ")";
}

void visitor::visit(const unary_expr& node) const{
	string code = to_string(node.tok.id);
	node.target->accept(*this);
	buffer = "(" + code + buffer + ")";
}

void visitor::visit(const function_expr& node) const{
	string code;
	node.param->accept(*this);
	if(node.name->operator[](0) == 'F' || node.name->operator[](0) == 'f'){
		int id = (int) calls.size();
		calls[id] = std::make_pair(buffer, *node.name);
		code = string("t") + to_string(id);
	}
	else{
		code = *node.name + "(" + buffer + ")";
	}
	buffer = code;
}

void visitor::visit(const variable& node) const{
	if(*node.name == current_parameter){
		if(node.idx1 != nullptr || node.idx2 != nullptr){
			std::cerr << "Parameter cannot be used as array" << std::endl;
			exit(-1);
		}
		buffer = "parameter";
	} 
	else{
		string code = "get_var(\"" + *node.name + "\", ";
		if(node.idx1 != nullptr){
			node.idx1->accept(*this);
			code += "to_index(" + buffer + "), ";
		}
		else{
			code += "-1, ";
		}
		if(node.idx2 != nullptr){
			node.idx2->accept(*this);
			code += "to_index(" + buffer + "))";
		}
		else{
			code += "-1)";
		}
		buffer = code;
	}
}

template<class T> void visitor::visit(const literal_expr<T>& node) const{
	buffer = "to_value(" + to_string(node.value) + ")";
}

template void visitor::visit<string*>(const literal_expr<string*>& node) const;
template void visitor::visit<int>(const literal_expr<int>& node) const;
template void visitor::visit<bool>(const literal_expr<bool>& node) const;

void get_var_idxs(const visitor* vis, string label, const expr* idx1, const expr* idx2){
	string code;
	if(idx1 != nullptr){
		solve_expr(vis, *idx1, label + "_idx1", "idx1");
		code += buffer;
		code += verify_index("idx1");
	}
	else{
		code += create_default_index("idx1");
	}
	if(idx2 != nullptr){		
		code += "push_value(idx1);\n";
		solve_expr(vis, *idx2, label + "_idx2", "idx2");
		code += buffer;
		code += verify_index("idx2");
		code += "idx1 = pop_value();\n";
	}
	else{
		code += create_default_index("idx2");
	}
	buffer = code;
}

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
		code += "value t" + to_string(t.first) + ";\n";
		code += "t" + to_string(t.first) + " = " + "get_return_value();\n";
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
	string code = "value " + idx + ";\n";
	code += idx + " = to_value(-1);\n";
	return code;
}
