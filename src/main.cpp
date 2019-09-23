#include "parser.h"
#include "visitor.h"

int main(){
	yyparse();
	ast::visitor vis;
	root->accept(vis);
	delete root;
}
