%{
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "lex.hpp"
#include "return_types.hpp"
#include "token.hpp"
#include "tree_nodes.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::vector;

using namespace ast;

program* root = nullptr;
int error_count = 0;

position get_pos(YYLTYPE yypos){
    return position{yypos.first_line, yypos.first_column};
}

void print_error_location(position pos){
    cerr << "Position in file: line " << pos.get_line() << ", column " << pos.get_column() << endl << endl;
}

void print_error_in_statement(string token_name, int line_id){
    cerr << "error message: syntax error at " << token_name << " statement, in line defined as " << line_id << ". ";
}

%}

%define parse.error verbose
%define parse.lac full
// %define api.pure full
%locations

%{
void yyerror(const char * s){
    if(string(s).find("expecting LEXEOF") != string::npos){
        return;
    }
    cerr << s;
    if(string(s).find("unexpected LEXERROR") != string::npos){
        cerr << ". Note: LEXERROR token is used whenever a lexical error occurs";
    }
    cerr << endl;
    error_count++;
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
%type <_print_list> expr_list
%type <_num_list> num_list
%type <_expr> number
%type <_var_list> variable_list

%token
ABS 1
AND 2
ATN 3
<_bool> BOOLEAN 4
<_name> CHAR 5
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
<_name> FLOAT 19
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
LEXEOF 0
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
program         : stmts                                             {root = $$ = $1;}
                ;

end             : INTEGER END extra                                 {$$ = new end_stmt(token(END, get_pos(@2))); $$->set_line($1);}
                ;

extra           : LEXEOF                                            {}
                | error LEXEOF                                      {yyerrok;}
                ;

stmts           : end                                               {$$ = new program{}; $$->push_front($1);}
                | stmt_decl stmts                                   {$$ = $2; if($1 != nullptr) $$->push_front($1);}
                ;

stmt_decl       : INTEGER stmt ENDL                                 {$$ = $2; $$->set_line($1);}
                | ENDL                                              {$$ = nullptr;}
                | INTEGER ENDL                                      {$$ = new empty_stmt(token(INTEGER, get_pos(@1))); $$->set_line($1);}
                | error_level_1                                     {$$ = nullptr;}
                ;

error_level_1   : error_level_2                                     {}
                | error ENDL                                        {yyerrok; cerr << "error message: statement of undefined line. "; print_error_location(get_pos(@1));}
                ;

error_level_2   : error_level_3                                     {}
                | INTEGER error ENDL                                {yyerrok; cerr << "error message: unknown statement of line defined as " << $1 << ". "; print_error_location(get_pos(@2));}
                ;
                
error_level_3   : INTEGER LET error ENDL                            {yyerrok; print_error_in_statement("LET", $1); print_error_location(get_pos(@2));}
                | INTEGER PRINT error ENDL                          {yyerrok; print_error_in_statement("PRINT", $1); print_error_location(get_pos(@2));}
                | INTEGER READ error ENDL                           {yyerrok; print_error_in_statement("READ", $1); print_error_location(get_pos(@2));}
                | INTEGER DATA error ENDL                           {yyerrok; print_error_in_statement("DATA", $1); print_error_location(get_pos(@2));}
                | INTEGER INPUT error ENDL                          {yyerrok; print_error_in_statement("INPUT", $1); print_error_location(get_pos(@2));}
                | INTEGER GOTO error ENDL                           {yyerrok; print_error_in_statement("GOTO", $1); print_error_location(get_pos(@2));}
                | INTEGER IF error ENDL                             {yyerrok; print_error_in_statement("IF", $1); print_error_location(get_pos(@2));}
                | INTEGER GOSUB error ENDL                          {yyerrok; print_error_in_statement("GOSUB", $1); print_error_location(get_pos(@2));}
                | INTEGER RETURN error ENDL                         {yyerrok; print_error_in_statement("RETURN", $1); print_error_location(get_pos(@2));}
                | INTEGER DEF error ENDL                            {yyerrok; print_error_in_statement("DEF", $1); print_error_location(get_pos(@2));}
                | INTEGER DIM error ENDL                            {yyerrok; print_error_in_statement("DIM", $1); print_error_location(get_pos(@2));}
                | INTEGER NEXT error ENDL                           {yyerrok; print_error_in_statement("NEXT", $1); print_error_location(get_pos(@2));}
                | INTEGER FOR error ENDL                            {yyerrok; print_error_in_statement("FOR", $1); print_error_location(get_pos(@2));}
                | INTEGER STOP error ENDL                           {yyerrok; print_error_in_statement("STOP", $1); print_error_location(get_pos(@2));}
                ;

stmt            : LET variable EQUALS expr                          {$$ = new let_stmt(token(LET, get_pos(@1)), $2, $4);}
                | PRINT expr_list                                   {$$ = new print_stmt(token(PRINT, get_pos(@1)), $2);}
                | READ  variable_list                               {$$ = new read_stmt(token(READ, get_pos(@1)), $2);}
                | DATA  num_list                                    {$$ = new data_stmt(token(DATA, get_pos(@1)), $2);}
                | INPUT variable_list                               {$$ = new input_stmt(token(INPUT, get_pos(@1)), $2);}
                | GOTO INTEGER                                      {$$ = new goto_stmt(token(GOTO, get_pos(@1)), $2);}
                | IF expr THEN INTEGER                              {$$ = new if_stmt(token(IF, get_pos(@1)), $2, $4);}
                | GOSUB INTEGER                                     {$$ = new gosub_stmt(token(GOSUB, get_pos(@1)), $2);}
                | RETURN                                            {$$ = new return_stmt(token(RETURN, get_pos(@1)));}
                | DEF FUNCTION LPAREN VARIABLE RPAREN EQUALS expr   {$$ = new def_stmt(token(DEF, get_pos(@1)), $2, $4, $7);}
                | DIM variable                                      {$$ = new dim_stmt(token(DIM, get_pos(@1)), $2);}
                | NEXT VARIABLE                                     {$$ = new next_stmt(token(NEXT, get_pos(@1)), new variable(token(VARIABLE, get_pos(@2)), $2));}
                | FOR VARIABLE EQUALS expr TO expr STEP expr        {$$ = new for_stmt(token(FOR, get_pos(@1)), new variable(token(VARIABLE, get_pos(@2)), $2), $4, $6, $8);}
                | FOR VARIABLE EQUALS expr TO expr                  {$$ = new for_stmt(token(FOR, get_pos(@1)), new variable(token(VARIABLE, get_pos(@2)), $2), $4, $6);}
                | STOP                                              {$$ = new stop_stmt(token(STOP, get_pos(@1)));}
                ;

num_list        : number                                            {$$ = new vector<expr*>({$1});}
                | num_list COMMA number                             {$$ = $1; $$->push_back($3);}
                ;

number          : INTEGER                                           {$$ = new literal_expr<int>(token(INTEGER, get_pos(@1)), $1);}
                | FLOAT                                             {$$ = new literal_expr<string*>(token(FLOAT, get_pos(@1)), $1);}
                | PLUS INTEGER                                      {$$ = new unary_expr(token(PLUS, get_pos(@1)), new literal_expr<int>(token(INTEGER, get_pos(@2)), $2));}
                | PLUS FLOAT                                        {$$ = new unary_expr(token(PLUS, get_pos(@1)), new literal_expr<string*>(token(FLOAT, get_pos(@2)), $2));}
                | MINUS INTEGER                                     {$$ = new unary_expr(token(MINUS, get_pos(@1)), new literal_expr<int>(token(INTEGER, get_pos(@2)), $2));}
                | MINUS FLOAT                                       {$$ = new unary_expr(token(MINUS, get_pos(@1)), new literal_expr<string*>(token(FLOAT, get_pos(@2)), $2));}
                ;

expr_list       : expr                                              {$$ = new vector<print_expr>({{$1, false}});}
                | expr_list COMMA expr                              {$$ = $1; $$->back().second = true; $$->push_back({$3, false});}
                | expr_list SEMICOLON expr                          {$$ = $1; $$->push_back({$3, false});}
                ;

variable_list   : variable                                          {$$ = new vector<variable*>({$1});}
                | variable_list COMMA variable                      {$$ = $1; $$->push_back($3);}
                ;

expr            : expr OR expr                                      {$$ = new binary_expr($1, token(OR, get_pos(@2)), $3);}
                | expr AND expr                                     {$$ = new binary_expr($1, token(AND, get_pos(@2)), $3);}
                | expr DIFF expr                                    {$$ = new binary_expr($1, token(DIFF, get_pos(@2)), $3);}
                | expr EQUALS expr                                  {$$ = new binary_expr($1, token(EQUALS, get_pos(@2)), $3);}
                | expr LT expr                                      {$$ = new binary_expr($1, token(LT, get_pos(@2)), $3);}
                | expr GT expr                                      {$$ = new binary_expr($1, token(GT, get_pos(@2)), $3);}
                | expr LTE expr                                     {$$ = new binary_expr($1, token(LTE, get_pos(@2)), $3);}
                | expr GTE expr                                     {$$ = new binary_expr($1, token(GTE, get_pos(@2)), $3);}
                | expr PLUS expr                                    {$$ = new binary_expr($1, token(PLUS, get_pos(@2)), $3);}
                | expr MINUS expr                                   {$$ = new binary_expr($1, token(MINUS, get_pos(@2)), $3);}
                | expr TIMES expr                                   {$$ = new binary_expr($1, token(TIMES, get_pos(@2)), $3);}
                | expr DIVIDE expr                                  {$$ = new binary_expr($1, token(DIVIDE, get_pos(@2)), $3);}
                | expr MOD expr                                     {$$ = new binary_expr($1, token(MOD, get_pos(@2)), $3);}
                | expr EXPONENTIAL expr                             {$$ = new binary_expr($1, token(EXPONENTIAL, get_pos(@2)), $3);}
                | LPAREN expr RPAREN                                {$$ = $2;}
                | native_function LPAREN expr RPAREN                {$$ = new function_expr(token(FUNCTION, get_pos(@1)), $1, $3);}
                | FUNCTION LPAREN expr RPAREN                       {$$ = new function_expr(token(FUNCTION, get_pos(@1)), $1, $3);}
                | NOT expr                                          {$$ = new unary_expr(token(NOT, get_pos(@1)), $2);}
                | MINUS expr %prec EXPONENTIAL                      {$$ = new unary_expr(token(MINUS, get_pos(@1)), $2);}
                | PLUS expr %prec EXPONENTIAL                       {$$ = new unary_expr(token(PLUS, get_pos(@1)), $2);}
                | variable                                          {$$ = $1;}
                | INTEGER                                           {$$ = new literal_expr<int>(token(INTEGER, get_pos(@1)), $1);}
                | FLOAT                                             {$$ = new literal_expr<string*>(token(FLOAT, get_pos(@1)), $1);}
                | STRING                                            {$$ = new literal_expr<string*>(token(CHAR, get_pos(@1)), $1);}
                | CHAR                                              {$$ = new literal_expr<string*>(token(CHAR, get_pos(@1)), $1);}
                | BOOLEAN                                           {$$ = new literal_expr<bool>(token(BOOLEAN, get_pos(@1)), $1);}
                ;

variable        : VARIABLE                                          {$$ = new variable(token(VARIABLE, get_pos(@1)), $1);}
                | VARIABLE LPAREN expr RPAREN                       {$$ = new variable(token(VARIABLE, get_pos(@1)), $1, $3);}
                | VARIABLE LPAREN expr COMMA expr RPAREN            {$$ = new variable(token(VARIABLE, get_pos(@1)), $1, $3, $5);}
                ;

native_function : ABS                                               {$$ = new string("ABS");}
                | ATN                                               {$$ = new string("ATN");}
                | COS                                               {$$ = new string("COS");}
                | EXP                                               {$$ = new string("EXP");}
                | INT                                               {$$ = new string("INT");}
                | LOG                                               {$$ = new string("LOG");}
                | RND                                               {$$ = new string("RND");}
                | SIN                                               {$$ = new string("SIN");}
                | SQR                                               {$$ = new string("SQR");}
                | TAN                                               {$$ = new string("TAN");}
                ;

%%

