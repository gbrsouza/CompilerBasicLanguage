#include "parser.hpp"
#include "visitor.hpp"

#include <iostream>
using namespace std;

int main(){
	yyparse();
	ast::visitor vis;
	root->accept(vis);
	delete root;
}
