#include "parser.hpp"
#include "visitor.hpp"

#include <iostream>
using namespace std;

int main(){
	yyparse();
	ast::visitor vis;
	if(root == nullptr || error_count > 0){
		if(error_count > 0){
			cerr << error_count << " syntax error(s) found" << endl;
		}
		if(root != nullptr){
			delete root;
		}
		return -1;
	}
	root->accept(vis);
	delete root;
}
