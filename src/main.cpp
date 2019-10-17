#include "parser.hpp"
#include "visitor.hpp"

#include <iostream>
using namespace std;

int main(){
	yyparse();
	ast::visitor vis;
	if(root == nullptr){
		std::cerr << "Syntax error!" << std::endl;
		return -1;
	}
	root->accept(vis);
	delete root;
}
