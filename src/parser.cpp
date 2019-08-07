#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "parser.h"
#include "symbol.h"
#include "token.h"

using std::cin;
using std::cout;
using std::map;
using std::set;
using std::string;
using std::vector;

extern int line, column;
extern char * text;

map<symbol, vector<vector<symbol> > > rules;
map<symbol, bool> nullable, endable;
map<symbol, set<token> > follow, first;

map<string, symbol> string_to_symbol = {
	{"ABS", T_ABS},
	{"AND", T_AND},
	{"ATN", T_ATN},
	{"BOOLEAN", T_BOOLEAN},
	{"CHAR", T_CHAR},
	{",", T_COMMA},
	{"COMMENT", T_COMMENT},
	{"COS", T_COS},
	{"DATA", T_DATA},
	{"DEF", T_DEF},
	{"<>", T_DIFF},
	{"DIM", T_DIM},
	{"/", T_DIVIDE},
	{"END", T_END},
	{"ENDL", T_ENDL},
	{"=", T_EQUALS},
	{"EXP", T_EXP},
	{"^", T_EXPONENTIAL},
	{"FLOAT", T_FLOAT},
	{"FOR", T_FOR},
	{"FUNCTION", T_FUNCTION},
	{"GOSUB", T_GOSUB},
	{"GOTO", T_GOTO},
	{">", T_GT},
	{">=", T_GTE},
	{"IF", T_IF},
	{"INPUT", T_INPUT},
	{"INT", T_INT},
	{"INTEGER", T_INTEGER},
	{"LET", T_LET},
	{"EOF", T_LEXEOF},
	{"LOG", T_LOG},
	{"(", T_LPAREN},
	{"<", T_LT},
	{"<=", T_LTE},
	{"-", T_MINUS},
	{"%", T_MOD},
	{"NEXT", T_NEXT},
	{"NOT", T_NOT},
	{"OR", T_OR},
	{"+", T_PLUS},
	{"PRINT", T_PRINT},
	{"READ", T_READ},
	{"RETURN", T_RETURN},
	{"RND", T_RND},
	{")", T_RPAREN},
	{";", T_SEMICOLON},
	{"SIN", T_SIN},
	{"SQR", T_SQR},
	{"STEP", T_STEP},
	{"STOP", T_STOP},
	{"STRING", T_STRING},
	{"TAN", T_TAN},
	{"THEN", T_THEN},
	{"*", T_TIMES},
	{"TO", T_TO},
	{"VARIABLE", T_VARIABLE},
	{"WHITE", T_WHITE},
	{"<program>", PROGRAM},
	{"<and_exp2>", ANDEXP2},
	{"<expo_exp2>", EXPOEXP2},
	{"<expr_list2>", EXPR_LIST2},
	{"<forinit>", FORINIT},
	{"<forstep>", FORSTEP},
	{"<num_list2>", NUM_LIST2},
	{"<forblock>", FORBLOCK},
	{"<numerated_for>", NUMERATED_FOR},
	{"<fornext>", FORNEXT},
	{"<forbody>", FORBODY},
	{"<numerated_main>", NUMERATED_MAIN},
	{"<numerated_block>", NUMERATED_BLOCK},
	{"<blocks>", BLOCKS},
	{"<or_exp>", OREXP},
	{"<and_exp>", ANDEXP},
	{"<or_exp2>", OREXP2},
	{"<expo_exp>", EXPOEXP},
	{"<prod_exp2>", PRODEXP2},
	{"<rel_exp>", RELEXP},
	{"<rel_exp2>", RELEXP2},
	{"<rel_op>", RELOP},
	{"<stmt>", STMT},
	{"<expr_list>", EXPR_LIST},
	{"<num_list>", NUM_LIST},
	{"<sum_exp>", SUMEXP},
	{"<prod_exp>", PRODEXP},
	{"<sum_exp2>", SUMEXP2},
	{"<unary_exp>", UNARYEXP},
	{"<native_function>", NATIVE_FUNCTION},
	{"<unary_op>", UNARYOP},
	{"<expr>", EXPR},
	{"<idx2>", IDX2},
	{"<variable2>", VARIABLE2},
	{"<variable_list>", VARIABLE_LIST},
	{"<var>", VAR},
	{"<variable_list2>", VARIABLE_LIST2},
	{"<empty_lines>", EMPTY_LINES},
};

map<symbol, string> symbol_to_string;

string convertToLatex(string s){
	string ret = "";
	for(int i = 0; i < s.size(); i++){
		if(s[i] == '_'){
			ret += "\\textunderscore ";
		}
		else if(s[i] == '<'){
			ret += "$<$";
		}
		else if(s[i] == '>'){
			ret += "$>$";
		}
		else{
			ret += s[i];
		}
	}
	return ret;
}

struct Node {
	Node* father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
	
	Node(symbol s, string l = "") : sym(s), lexeme(l) { }
};

string createTikzTree(Node *root, int depth=0){
	if(root == NULL) return "";
	string ret = "";
	if(depth == 0){
		string s = (root->lexeme == "" ? symbol_to_string[root->sym] : root->lexeme);
		ret += "\\node{" + convertToLatex(s) + "}";
		for(auto child : root->children){
			ret += "child {";
			ret += createTikzTree(child, depth+1);
			ret += "}";
		}
	}
	else{
		//for(int i = 0; i < depth; i++) ret += "\t";
		string s = (root->lexeme == "" ? symbol_to_string[root->sym] : root->lexeme);
		ret += "node{" + convertToLatex(s) + "}";
		for(auto child : root->children){
			//for(int i = 0; i < depth; i++) ret += "\t";
			ret += "child {";
			ret += createTikzTree(child, depth+1);
			//for(int i = 0; i < depth; i++) ret += "\t";
			ret += "}";
		}
	}
	return ret;
}

void build_sets(){
	std::ifstream in("documentation/table.txt");
	std::streambuf *cinbuf = cin.rdbuf(); //save old buf
	cin.rdbuf(in.rdbuf());
	
	string Cur;
	cin >> Cur;
	assert(Cur == "#");
	while((cin >> Cur) && Cur != "#");
	
	while(cin >> Cur){
		assert(string_to_symbol.count(Cur));
		symbol cur = string_to_symbol[Cur];
		string s;
		cin >> s;
		assert(s == "|");
		set<token> fst, flw;
		while((cin >> s) && s != "|"){
			assert(string_to_symbol.count(s) && string_to_symbol[s] > 0);
			fst.insert((token)string_to_symbol[s]);
		}
		while((cin >> s) && s != "|"){
			assert(string_to_symbol.count(s) && string_to_symbol[s] > 0);
			flw.insert((token)string_to_symbol[s]);
		}
		first[cur] = fst;
		follow[cur] = flw;
		cin >> s;
		assert(s == "yes" || s == "no");
		nullable[cur] = (s == "yes");
		cin >> s;
		assert(s == "|");
		cin >> s;
		assert(s == "yes" || s == "no");
		endable[cur] = (s == "yes");
		cin >> s;
		assert(s == "|");
	}
	
	cin.rdbuf(cinbuf);
}

void read_grammar(){
	std::ifstream in("documentation/grammar.txt");
	std::streambuf *cinbuf = cin.rdbuf(); //save old buf
	cin.rdbuf(in.rdbuf());

	string s;
	vector<string> input;
	vector<int> ids;
	while(cin >> s){
		if(s == "::="){
			ids.push_back((int) input.size());
		}
		input.push_back(s);
	}
	for(int i = 0; i < (int)ids.size(); i++){
		int p = ids[i], end;
		if(i + 1 == (int) ids.size()){
			end = (int) input.size();
		}
		else{
			end = ids[i+1] - 1;
		}
		assert(string_to_symbol.count(input[p-1]));
		symbol cur = string_to_symbol[input[p-1]];
		rules[cur] = vector<vector<symbol> >(0);
		vector<symbol> rule;
		bool has_lambda = false;
		for(int j = p + 1; j < end; j++){
			if(input[j] == "LAMBDA"){
				has_lambda = true;
				continue;
			}
			if(input[j] != "|"){
				assert(string_to_symbol.count(input[j]));
				rule.push_back(string_to_symbol[input[j]]);
			}
			else{
				rules[cur].push_back(rule);
				rule.clear();
			}
		}
		if(!rule.empty() || has_lambda){
			rules[cur].push_back(rule);
		}
	}
	
	cin.rdbuf(cinbuf);
}

void init_parser(){
	for(auto it : string_to_symbol){
		symbol_to_string[it.second] = it.first;
	}
	build_sets();
	read_grammar();
}

token next_useful_token(){
	token tok = next_token();
	while(tok == WHITE || tok == COMMENT){
		tok = next_token();
	}
	if(tok == LEXERROR){
		cout << "Lexical error!\n";
		cout << "line: " << line << "\n";
		cout << "column: " << column << "\n";
		cout << "text: " << text << "\n";
		exit(0);
	}
	return tok;
}

struct Node {
	Node* father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
	
	Node(symbol s, string l = "") : sym(s), lexeme(l) { }
};

void link(Node* father, Node* child){
	if(father != NULL){
		father->children.push_back(child);
	}
	child->father = father;
}

Node* get_terminal_node(token nxt){
	string lexeme = "";
	switch(nxt){
		case BOOLEAN:
		case CHAR:
		case DIFF:
		case DIVIDE:
		case EQUALS:
		case GT:
		case GTE:
		case INTEGER:
		case FLOAT:
		case LPAREN:
		case LT:
		case LTE:
		case MOD:
		case PLUS:
		case RPAREN:
		case STRING:
		case TIMES:
		case VARIABLE:
			lexeme = string(text);
		default:
			break;
	}
	return new Node((symbol) nxt, lexeme);
}

Node* run_recursive_parser(symbol sym, token& nxt){
	Node* root = new Node(sym);
	for(auto & rule : rules[sym]){
		if(rule.empty()){
			return root;
		}
		if(rule[0] == (symbol) nxt || (rule[0] < 0 && first[rule[0]].count(nxt))
			|| (rule[0] < 0 && nullable[rule[0]] && follow[rule[0]].count(nxt)) ){
			
			for(int i = 0; i < (int)rule.size(); i++){
				if(rule[i] < 0){
					Node* child = run_recursive_parser(rule[i], nxt);
					link(root, child);
				}
				else{
					if((token) rule[i] != nxt){
						cout << "Syntax error!\n";
						cout << "line: " << line << "\n";
						cout << "column: " << column << "\n";
						cout << "text: " << text << "\n";
						cout << "expected token id: " << symbol_to_string[rule[i]] << "\n";
						cout << "actual token id: " << symbol_to_string[(symbol) nxt] << "\n";
						exit(0);
					}
					else{ // match
						bool relevant = nxt != ENDL && nxt != LPAREN && nxt != RPAREN;
						relevant &= (sym == EXPR_LIST2) || (nxt != COMMA && nxt != SEMICOLON);
						if(relevant){ // ignore irrelevant tokens
							link(root, get_terminal_node(nxt));
						}
						nxt = next_useful_token();
					}
				}
			}
			
			return root;
		}
	}
	
	cout << "Syntax error!\n";
	cout << "Couldn't derive any expression from non-terminal symbol " << symbol_to_string[sym] << "\n";
	cout << "line: " << line << "\n";
	cout << "column: " << column << "\n";
	cout << "text: " << text << "\n";
	cout << "token id: " << symbol_to_string[(symbol) nxt] << "\n";
	exit(0);
}

void compress_tree(Node* node){
	bool should_be_compressed = false;
	switch(node->sym){
		case EMPTY_LINES:
		case ANDEXP2:
		case EXPOEXP2:
		case EXPR_LIST2:
		case NUM_LIST2:
		case OREXP2:
		case PRODEXP2:
		case RELEXP2:
		case SUMEXP2:
		case IDX2:
		case VARIABLE2:
		case VARIABLE_LIST2:
		case UNARYOP:
		case RELOP:
		case NATIVE_FUNCTION:
			should_be_compressed = true;
		default:
			break;
	}
	if(node->father != NULL && node->sym == node->father->sym){
		should_be_compressed = true;
	}
	for(Node* child : node->children){
		if(should_be_compressed){
			child->father = node->father;
		}
		compress_tree(child);
	}
	if(node->sym < 0 && node->children.empty()){
		should_be_compressed = true;
	}
	if(should_be_compressed){
		node->father = NULL;
	}
}

void update_children(Node* node, bool is_root = false){
	vector<Node*> old_children = node->children;
	node->children.clear();
	
	for(Node* child : old_children){
		update_children(child);
	}
	if(node->sym < 0 && node->children.empty()){
		node->father = NULL;
	}
	if(node->father == NULL && !is_root){
		delete node;
	}
	if(node->father != NULL){
		node->father->children.push_back(node);
	}
}

Node* copy_node(Node* node){
	Node* cp = new Node(node->sym);
	*cp = *node;
	return cp;
}

Node* build_abstract_tree(Node* node, Node* abstract_father){
	int assoc = 0; // left = -1, none = 0, right = 1
	switch(node->sym){
		case NUM_LIST:
		case EXPR_LIST:
		case VARIABLE_LIST:
		case EXPOEXP:
			assoc = 1;
			break;
		case OREXP:
		case ANDEXP:
		case SUMEXP:
		case PRODEXP:
			assoc = -1;
		default:
			break;
	}
	bool ignore = false;
	if(node->children.size() == 1){
		switch(node->sym){
			case OREXP:
			case ANDEXP:
			case RELEXP:
			case SUMEXP:
			case PRODEXP:
			case EXPOEXP:
			case UNARYEXP:
				ignore = true;
			default:
				break;
		}
	}
	Node* abstract_node;
	if(ignore){
		abstract_node = abstract_father;
	}
	else{
		abstract_node = copy_node(node);
		abstract_node->children.clear();
		link(abstract_father, abstract_node);
	}
	if(assoc == 0 || node->children.size() < 3){
		for(Node* child : node->children){
			build_abstract_tree(child, abstract_node);
		}
	}
	else{
		symbol sym = node->sym;
		int step = 1;
		if(sym != NUM_LIST && sym != VARIABLE_LIST){
			step = 2;
		}
		size_t number_new_nodes = (node->children.size() - 1) / step;
		vector<Node*> auxiliary_nodes(number_new_nodes, NULL);
		if(assoc < 0){
			size_t j = 0;
			for(size_t i = 1; i < node->children.size(); i += step){
				if(j < number_new_nodes - 1){
					auxiliary_nodes[j] = new Node(node->sym, node->lexeme);
				}
				else{
					auxiliary_nodes[j] = abstract_node;
				}
				if(i == 1){
					build_abstract_tree(node->children[0], auxiliary_nodes[j]);
				}
				else{
					link(auxiliary_nodes[j], auxiliary_nodes[j - 1]);
				}
				for(int i2 = 0; i2 < step; i2++){
					build_abstract_tree(node->children[i + i2], auxiliary_nodes[j]);
				}
				j++;
			}
		}
		else{
			size_t j = number_new_nodes - 1;
			for(size_t i = node->children.size() - 1 - step; i + step > i; i -= step){
				if(j > 0){
					auxiliary_nodes[j] = new Node(node->sym, node->lexeme);
				}
				else{
					auxiliary_nodes[j] = abstract_node;
				}
				for(int i2 = 0; i2 < step; i2++){
					build_abstract_tree(node->children[i + i2], auxiliary_nodes[j]);
				}
				if(i == node->children.size() - 2){
					build_abstract_tree(node->children.back(), auxiliary_nodes[j]);
				}
				else{
					link(auxiliary_nodes[j], auxiliary_nodes[j + 1]);
				}
				j--;
			}
		}
	}
	
	return abstract_node;
}

void delete_tree(Node * root){
	for(Node* child : root->children){
		delete_tree(child);
	}
	delete root;
}

void run_recursive_parser(){
	token nxt_token = next_useful_token();
	Node* tree = run_recursive_parser(PROGRAM, nxt_token);
	
	compress_tree(tree);
	update_children(tree, true);
	
	Node* abstract_tree = build_abstract_tree(tree, NULL);
	delete_tree(tree);
}

void run_parser_with_table(){
	//TODO
}
