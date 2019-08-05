#include <iostream>
#include <map>
#include <string>

#include<bits/stdc++.h>
#define sz(a) (int) ((a).size())
#define pb push_back
using namespace std;

#include "token.h"
#include "symbol.h"

using std::cout;
using std::endl;
using std::string;
using std::map;

map<token, string> token_name = {
	{ABS, "ABS"},
	{AND, "AND"},
	{ATN, "ATN"},
	{BOOLEAN, "BOOLEAN"},
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

map<string, symbol> to_symbol = {
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
};

map<symbol, vector<vector<symbol> > > rules;
map<symbol, bool> nullable, endable;
map<symbol, set<symbol> > follow, first;

void build_sets(){
	ifstream in("documentation/table.txt");
	streambuf *cinbuf = cin.rdbuf(); //save old buf
	cin.rdbuf(in.rdbuf());
	
	string Cur;
	cin >> Cur;
	assert(Cur == "#");
	while((cin >> Cur) && Cur != "#");
	
	while(cin >> Cur){
		if(!to_symbol.count(Cur)){
			cout << Cur << endl;
		}
		symbol cur = to_symbol[Cur];
		string s;
		cin >> s;
		assert(s == "|");
		set<symbol> fst, flw;
		while((cin >> s) && s != "|"){
			if(!to_symbol.count(s)){
				cout << s << endl;
			}
			fst.insert(to_symbol[s]);
		}
		while((cin >> s) && s != "|"){
			if(!to_symbol.count(s)){
				cout << s << endl;
			}
			flw.insert(to_symbol[s]);
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
	ifstream in("documentation/grammar.txt");
	streambuf *cinbuf = cin.rdbuf(); //save old buf
	cin.rdbuf(in.rdbuf());

	string s;
	vector<string> input;
	vector<int> ids;
	while(cin >> s){
		if(s == "::="){
			ids.pb(sz(input));
		}
		input.pb(s);
	}
	for(int i = 0; i < sz(ids); i++){
		int p = ids[i], end;
		if(i + 1 == sz(ids)) end = sz(input);
		else end = ids[i+1] - 1;
		if(!to_symbol.count(input[p-1])){
			cout << input[p-1] << endl;
		}
		symbol cur = to_symbol[input[p-1]];
		rules[cur] = vector<vector<symbol> >(0);
		vector<symbol> rule;
		for(int j = p + 1; j < end; j++){
		    if(input[j] == "LAMBDA") continue;
			if(input[j] != "|"){
				if(!to_symbol.count(input[j])) cout << "~ " << input[j] << endl;
				rule.pb(to_symbol[input[j]]);
			}
			else{
				rules[cur].pb(rule);
				rule.clear();
			}
		}
		if(!rule.empty()) rules[cur].pb(rule);
	}
	
	cin.rdbuf(cinbuf);
}

void run_lexical(){
	token cur;
	while((cur = nextToken()) != LEXEOF) {
		if(cur == WHITE){
			continue;
		}
		
		cout << "TokenId: " << token_name[cur] << endl;
		cout << "Line: " << line << endl;
		cout << "Column: " << column << endl;
		if(cur == FLOAT || cur == INTEGER || cur == FUNCTION || cur == VARIABLE || 
		   cur == STRING || cur == CHAR || cur == LEXERROR){
			cout << "Text: " << text << endl;
		}
		
		cout << endl;
	}
}

int main(){
	build_sets();
	read_grammar();
}
