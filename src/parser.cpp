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
	{"<empty_lines>", EMPTY_LINES},
};

map<symbol, string> to_string;

void build_sets(){
	std::ifstream in("documentation/table.txt");
	std::streambuf *cinbuf = cin.rdbuf(); //save old buf
	cin.rdbuf(in.rdbuf());
	
	string Cur;
	cin >> Cur;
	assert(Cur == "#");
	while((cin >> Cur) && Cur != "#");
	
	while(cin >> Cur){
		assert(to_symbol.count(Cur));
		symbol cur = to_symbol[Cur];
		string s;
		cin >> s;
		assert(s == "|");
		set<token> fst, flw;
		while((cin >> s) && s != "|"){
			assert(to_symbol.count(s) && to_symbol[s] > 0);
			fst.insert((token)to_symbol[s]);
		}
		while((cin >> s) && s != "|"){
			assert(to_symbol.count(s) && to_symbol[s] > 0);
			flw.insert((token)to_symbol[s]);
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
		assert(to_symbol.count(input[p-1]));
		symbol cur = to_symbol[input[p-1]];
		rules[cur] = vector<vector<symbol> >(0);
		vector<symbol> rule;
		bool has_lambda = false;
		for(int j = p + 1; j < end; j++){
		    if(input[j] == "LAMBDA"){
		        has_lambda = true;
		        continue;
	        }
			if(input[j] != "|"){
				assert(to_symbol.count(input[j]));
				rule.push_back(to_symbol[input[j]]);
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
    for(auto it : to_symbol){
        to_string[it.second] = it.first;
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

void run_recursive_parser(symbol sym, token& nxt){
    for(auto & rule : rules[sym]){
        if(rule.empty()){
            return;
        }
        if(rule[0] == (symbol) nxt || (rule[0] < 0 && first[rule[0]].count(nxt))
            || (rule[0] < 0 && nullable[rule[0]] && follow[rule[0]].count(nxt)) ){
            
            for(int i = 0; i < (int)rule.size(); i++){
                if(rule[i] < 0){
                    run_recursive_parser(rule[i], nxt);
                }
                else{
                    if((token) rule[i] != nxt){
                        cout << "Syntax error!\n";
                        cout << "line: " << line << "\n";
                        cout << "column: " << column << "\n";
                        cout << "text: " << text << "\n";
                        cout << "expected token id: " << to_string[rule[i]] << "\n";
                        cout << "actual token id: " << to_string[(symbol) nxt] << "\n";
                        exit(0);
                    }
                    else{
                        // match
                        nxt = next_useful_token();
                    }
                }
            }
            
            return;
        }
    }
    
    cout << "Syntax error!\n";
    cout << "Couldn't derive any expression from non-terminal symbol " << to_string[sym] << "\n";
    cout << "line: " << line << "\n";
    cout << "column: " << column << "\n";
    cout << "text: " << text << "\n";
    cout << "token id: " << to_string[(symbol) nxt] << "\n";
    exit(0);
}

void run_recursive_parser(){
    token nxt_token = next_useful_token();
    run_recursive_parser(PROGRAM, nxt_token);
}

void run_parser_with_table(){

}
