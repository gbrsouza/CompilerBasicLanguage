#ifndef RETURN_TYPES_H
#define RETURN_TYPES_H

#include <string>

#include "tree_nodes.h"

typedef union{
	ast::program* _program;
	ast::stmt* _stmt;
	ast::expr* _expr;
	ast::variable* _variable;
	std::string* _name;
	int _int;
} YYSTYPE;

extern YYSTYPE yylval;

int yyparse (void);

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

#endif
