%{
#include <string>
#include <vector>
#include <utility>

#include "lex.h"
#include "token.h"
#include "tree_nodes.h"

using std::string;
using std::vector;
using std::pair;

using namespace ast;

program* root = nullptr;

void yyerror(const char *){ }

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

expr            : expr OR expr
                | expr AND expr
                | expr DIFF expr
                | expr EQUALS expr
                | expr LT expr
                | expr GT expr
                | expr LTE expr
                | expr GTE expr
                | expr PLUS expr
                | expr MINUS expr
                | expr TIMES expr
                | expr DIVIDE expr
                | expr MOD expr
                | expr EXPONENTIAL expr
                | LPAREN expr RPAREN
                | native_function LPAREN expr RPAREN
                | FUNCTION LPAREN expr RPAREN
                | NOT expr
                | MINUS expr %prec EXPONENTIAL
                | PLUS expr %prec EXPONENTIAL
                | variable
                | INTEGER
                | FLOAT
                | STRING
                | CHAR
                | BOOLEAN
                ;

variable        : VARIABLE
                | VARIABLE LPAREN expr RPAREN
                | VARIABLE LPAREN expr COMMA expr RPAREN
                ;

native_function : ABS
                | ATN
                | COS
                | EXP
                | INT
                | LOG
                | RND
                | SIN
                | SQR
                | TAN
                ;

%%

