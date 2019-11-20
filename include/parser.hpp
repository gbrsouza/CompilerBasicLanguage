#ifndef PARSER
#define PARSER

#include "tree_nodes.hpp"

extern ast::program* root;
extern int error_count;

void yyparse();

#endif
