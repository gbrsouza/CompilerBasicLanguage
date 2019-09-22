#include "parser.h"

int main(){
	yyparse();
	delete root;
}
