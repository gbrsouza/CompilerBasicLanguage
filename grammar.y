%union {
	bool shit;
}

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
FUNCTION 21
GOSUB 22
GOTO 23
GT 24
GTE 25
IF 26
INPUT 27
INT 28
INTEGER 29
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
STRING 53
TAN 54
THEN 55
TIMES 56
TO 57
VARIABLE 58
WHITE 59
EOF 60

%%
program         : blocks end { shit = true; }
                ;

end             : INTEGER END empty_lines EOF
                ;

empty_lines     : ENDL empty_lines
                | ENDL
                ;

blocks          : block 
                | blocks block
                ;

block           : INTEGER stmt ENDL 
                | INTEGER ENDL 
                | ENDL 
                | for_block
                ;

for_block       : INTEGER for_init ENDL blocks next_stmt
                | INTEGER for_init ENDL next_stmt
                ;

for_init        : FOR variable EQUALS expr TO expr STEP expr
                | FOR variable EQUALS expr TO expr
                ;

next_stmt       : INTEGER NEXT variable ENDL
                ;

stmt            : LET variable EQUALS expr
                | PRINT expr_list
                | READ  variable_list
                | DATA  num_list
                | GOTO INTEGER
                | IF expr THEN INTEGER
                | GOSUB INTEGER
                | RETURN
                | DEF FUNCTION LPAREN VARIABLE RPAREN EQUALS expr
                | DIM variable
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

expr            : or_exp
                ;
                    
or_exp          : or_exp OR and_exp
                | and_exp
                ;

and_exp         : and_exp AND rel_exp
                | rel_exp
                ;

rel_exp         : sum_exp rel_op sum_exp
                | sum_exp
                ;

sum_exp         : sum_exp PLUS prod_exp
                | sum_exp MINUS prod_exp
                | prod_exp
                ;

prod_exp        : prod_exp TIMES expo_exp
                | prod_exp DIVIDE expo_exp
                | prod_exp MOD expo_exp
                | expo_exp
                ;

expo_exp        : unary_exp EXPONENTIAL expo_exp 
                | unary_exp
                ;

unary_exp       : INTEGER
                | FLOAT
                | variable
                | STRING
                | CHAR
                | BOOLEAN
                | unary_op unary_exp
                | LPAREN expr RPAREN
                | FUNCTION LPAREN expr RPAREN
                | native_function LPAREN expr RPAREN
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

unary_op        : MINUS
                | PLUS
                | NOT
                ;

rel_op          : DIFF
                | EQUALS
                | LT
                | GT
                | LTE
                | GTE
                ;

%%

int yywrap(){
    return 1;
}

int main() {
    auto ret = yyparse();
    if (root != nullptr) {
    }
    return ret;
}
