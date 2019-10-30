#include "parser.hpp"
#include "visitor.hpp"

#include <iostream>
using namespace std;

int main(){
	yyparse();
	ast::visitor vis;
	if(root == nullptr){
		return -1;
	}
	root->accept(vis);
	delete root;
}
