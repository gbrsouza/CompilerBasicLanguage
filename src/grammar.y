%{
#include <string>
#include <vector>
#include <utility>

#include "lex.h"
#include "return_types.h"
#include "token.h"
#include "tree_nodes.h"

using std::string;
using std::vector;
using std::pair;

using namespace ast;

program* root = nullptr;

void yyerror(const char *){ }

template<class T>
position get_pos(T yypos){
	return position{yypos.first_line, yypos.first_column};
}

%}

%type <_program> program
%type <_stmt> end
%type <_program> stmts
%type <_stmt> stmt_decl
%type <_stmt> stmt
%type <_expr> expr
%type <_variable> variable
%type <_name> native_function

%token
ABS 1
AND 2
ATN 3
BOOLEAN 4
CHAR 5
COMMA 6
COMMENT 7
COS 8
DATA 9
DEF 10
DIFF 11
DIM 12
DIVIDE 13
END 14
ENDL 15
EQUALS 16
EXP 17
EXPONENTIAL 18
FLOAT 19
FOR 20
<_name> FUNCTION 21
GOSUB 22
GOTO 23
GT 24
GTE 25
IF 26
INPUT 27
INT 28
<_int> INTEGER 29
LET 30
LEXEOF 31
LEXERROR 32
LOG 33
LPAREN 34
LT 35
LTE 36
MINUS 37
MOD 38
NEXT 39
NOT 40
OR 41
PLUS 42
PRINT 43
READ 44
RETURN 45
RND 46
RPAREN 47
SEMICOLON 48
SIN 49
SQR 50
STEP 51
STOP 52
<_name> STRING 53
TAN 54
THEN 55
TIMES 56
TO 57
<_name> VARIABLE 58
WHITE 59

%start program

// Operator's associativity and precedence
%left OR
%left AND
%nonassoc DIFF EQUALS LT GT LTE GTE
%left PLUS MINUS
%left TIMES DIVIDE MOD
%right EXPONENTIAL NOT

%%
program         : stmts			{root = $$ = $1;}
                ;

end             : INTEGER END	{$$ = new stmt{}; $$->set_line($1);}
                ;

stmts           : end			{$$ = new program{}; $$->push_front($1); }
                | stmt_decl stmts {$$ = $2; if($1 != nullptr) $$->push_front($1);}
                ;

stmt_decl       : INTEGER stmt ENDL	{$$ = $2; $$->set_line($1);}
                | ENDL				{$$ = nullptr;}
                | INTEGER ENDL		{$$ = nullptr;}
                ;

stmt            : LET variable EQUALS expr	{$$ = new let_stmt($2, $4);}
                | PRINT expr_list			
                | READ  variable_list
                | DATA  num_list
                | GOTO INTEGER
                | IF expr THEN INTEGER
                | GOSUB INTEGER
                | RETURN
                | DEF FUNCTION LPAREN VARIABLE RPAREN EQUALS expr
                | DIM variable
                | NEXT variable
                | FOR variable EQUALS expr TO expr STEP expr
                | FOR variable EQUALS expr TO expr
                | STOP
                ;

num_list        : INTEGER
                | FLOAT
                | num_list COMMA INTEGER
                | num_list COMMA FLOAT
                ;

expr_list       : expr
                | expr_list COMMA expr
                | expr_list SEMICOLON expr
                ;

variable_list   : variable
                | variable_list COMMA variable
                ;

expr            : expr OR expr			{$$ = new binary_expr($1, token(OR, get_pos(@2)), $3);}
                | expr AND expr			{$$ = new binary_expr($1, token(AND, get_pos(@2)), $3);}
                | expr DIFF expr		{$$ = new binary_expr($1, token(DIFF, get_pos(@2)), $3);}
                | expr EQUALS expr		{$$ = new binary_expr($1, token(EQUALS, get_pos(@2)), $3);}
                | expr LT expr			{$$ = new binary_expr($1, token(LT, get_pos(@2)), $3);}
                | expr GT expr			{$$ = new binary_expr($1, token(GT, get_pos(@2)), $3);}
                | expr LTE expr			{$$ = new binary_expr($1, token(LTE, get_pos(@2)), $3);}
                | expr GTE expr			{$$ = new binary_expr($1, token(GTE, get_pos(@2)), $3);}
                | expr PLUS expr		{$$ = new binary_expr($1, token(PLUS, get_pos(@2)), $3);}
                | expr MINUS expr		{$$ = new binary_expr($1, token(MINUS, get_pos(@2)), $3);}
                | expr TIMES expr		{$$ = new binary_expr($1, token(TIMES, get_pos(@2)), $3);}
                | expr DIVIDE expr		{$$ = new binary_expr($1, token(DIVIDE, get_pos(@2)), $3);}
                | expr MOD expr			{$$ = new binary_expr($1, token(MOD, get_pos(@2)), $3);}
                | expr EXPONENTIAL expr	{$$ = new binary_expr($1, token(EXPONENTIAL, get_pos(@2)), $3);}
                | LPAREN expr RPAREN	{$$ = $2;}
                | native_function LPAREN expr RPAREN {$$ = new function_expr(token(FUNCTION, get_pos(@1)), $1, $3);}
                | FUNCTION LPAREN expr RPAREN {$$ = new function_expr(token(FUNCTION, get_pos(@1)), $1, $3);}
                | NOT expr				{$$ = new unary_expr(token(NOT, get_pos(@1)), $2);}
                | MINUS expr %prec EXPONENTIAL {$$ = new unary_expr(token(MINUS, get_pos(@1)), $2);}
                | PLUS expr %prec EXPONENTIAL {$$ = new unary_expr(token(PLUS, get_pos(@1)), $2);}
                | variable				{$$ = $1;}
                | INTEGER				{$$ = new literal_expr<int>(token(INTEGER, get_pos(@1)), $1);}
                | FLOAT
                | STRING				{$$ = new literal_expr<string*>(token(STRING, get_pos(@1)), $1);}
                | CHAR
                | BOOLEAN
                ;

variable        : VARIABLE
                | VARIABLE LPAREN expr RPAREN
                | VARIABLE LPAREN expr COMMA expr RPAREN
                ;

native_function : ABS	{$$ = new string("ABS");}
                | ATN	{$$ = new string("ATN");}
                | COS	{$$ = new string("COS");}
                | EXP	{$$ = new string("EXP");}
                | INT	{$$ = new string("INT");}
                | LOG	{$$ = new string("LOG");}
                | RND	{$$ = new string("RND");}
                | SIN	{$$ = new string("SIN");}
                | SQR	{$$ = new string("SQR");}
                | TAN	{$$ = new string("TAN");}
                ;

%%

