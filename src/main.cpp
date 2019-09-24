#include "parser.hpp"
#include "visitor.hpp"

int main(){
	yyparse();
	ast::visitor vis;
	root->accept(vis);
	delete root;
}
