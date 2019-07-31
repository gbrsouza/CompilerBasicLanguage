%{ 

#include<bits/stdc++.h>
using namespace std;

#define ABS 20
#define ATN 21
#define COMMENT 17
#define COS 22
#define DATA 6
#define DEF 15
#define DIM 16
#define DIVIDE 33
#define END 3
#define ENDL 48
#define EQUALS 39
#define EXP 23
#define EXPONENTIAL 34
#define FOR 9
#define FUNCTION 42
#define GOSUB 13
#define GOTO 7
#define GT 35
#define GTE 37
#define IF 8
#define INPUT 5
#define INT 24
#define INTEGER 44
#define LET 1
#define LEXEOF 50
#define LEXERROR 47
#define LOG 25
#define LPAREN 40
#define LT 36
#define LTE 38
#define MINUS 31
#define NEXT 12
#define PLUS 30
#define PRINT 2
#define READ 4
#define RETURN 14
#define RND 26
#define RPAREN 41
#define SEMICOLON 46
#define SIN 27
#define SQR 28
#define STEP 11
#define STOP 18
#define STRING 45
#define TAN 29
#define THEN 19
#define TIMES 32
#define TO 10
#define VARIABLE 43
#define WHITE 49
#define AND 51
#define OR 52
#define NOT 53
#define DIFF 54
#define FLOAT 55
#define BOOL 56
#define CHAR 57
#define COMMA 58

map<char, string> trad = {{20, "ABS"},
				{21, "ATN"},
				{17, "COMMENT"},
				{22, "COS"},
				{6, "DATA"},
				{15, "DEF"},
				{16, "DIM"},
				{33, "DIVIDE"},
				{3, "END"},
				{48, "ENDL"},
				{39, "EQUALS"},
				{23, "EXP"},
				{34, "EXPONENTIAL"},
				{9, "FOR"},
				{42, "FUNCTION"},
				{13, "GOSUB"},
				{7, "GOTO"},
				{35, "GT"},
				{37, "GTE"},
				{8, "IF"},
				{5, "INPUT"},
				{24, "INT"},
				{44, "INTEGER"},
				{1, "LET"},
				{50, "LEXEOF"},
				{47, "LEXERROR"},
				{25, "LOG"},
				{40, "LPAREN"},
				{36, "LT"},
				{38, "LTE"},
				{31, "MINUS"},
				{12, "NEXT"},
				{30, "PLUS"},
				{2, "PRINT"},
				{4, "READ"},
				{14, "RETURN"},
				{26, "RND"},
				{41, "RPAREN"},
				{46, "SEMICOLON"},
				{27, "SIN"},
				{28, "SQR"},
				{11, "STEP"},
				{18, "STOP"},
				{45, "STRING"},
				{29, "TAN"},
				{19, "THEN"},
				{32, "TIMES"},
				{10, "TO"},
				{43, "VARIABLE"},
				{49, "WHITE"},
				{51, "AND"},
				{52, "OR"},
				{53, "NOT"},
				{54, "DIFF"},
				{55, "FLOAT"},
				{56, "BOOL"},
				{57, "CHAR"},
				{58, "COMMA"}
};

int line = 1, column = 1;

%} 
  
/* Rules Section*/
%% 
[ \t\r]								{return WHITE;}
\n									{return ENDL;}
AND									{return AND;}
OR									{return OR;}
NOT									{return NOT;}
LET									{return LET;} 
PRINT								{return PRINT;}
END									{return END;}
READ								{return READ;}
INPUT								{return INPUT;}
DATA								{return DATA;}
GOTO								{return GOTO;}
IF									{return IF;}
FOR									{return FOR;}
TO									{return TO;}
STEP								{return STEP;}
NEXT								{return NEXT;}
GOSUB								{return GOSUB;}
RETURN								{return RETURN;}
DEF									{return DEF;}
DIM									{return DIM;}
REM[ ](.*)							{return COMMENT;}
STOP								{return STOP;}
THEN								{return THEN;}
TRUE								{return BOOL;}
FALSE								{return BOOL;}

ABS									{return ABS;}
ATN									{return ATN;}
COS									{return COS;}
EXP									{return EXP;}
INT									{return INT;}
LOG									{return LOG;}
RND									{return RND;}
SIN									{return SIN;}
SQR									{return SQR;}
TAN									{return TAN;}

\<>									{return DIFF;}
\+									{return PLUS;}
\-									{return MINUS;}
\*									{return TIMES;}
\/									{return DIVIDE;}
\^									{return EXPONENTIAL;}
>									{return GT;}
\<									{return LT;}
>=									{return GTE;}
\<=									{return LTE;}
=									{return EQUALS;}
\(									{return LPAREN;}
\)									{return RPAREN;}
,									{return COMMA;}

FN[A-Z]								{return FUNCTION;}
[A-Z]([0-9]?)						{return VARIABLE;}


[0-9]*\.[0-9]+						{return FLOAT;}
[0-9]+								{return INTEGER;}
\"[^\"\\]*(\\.[^\"\\]*)*\"			{return STRING;}
\'(\\n|\\t|\\r|\\0|\\'|\\\\|.)\'	{return CHAR;}
;									{return SEMICOLON;}
.									{return LEXERROR;}
<<EOF>>								{return LEXEOF;}


%% 

int yywrap(void){}

char nextToken(){
	char p = yylex();
	if(p == ENDL){
		line++;
		column = 1;
	}
	else if(p != LEXEOF && p != WHITE){
		cout << "text: " << yytext << endl;
		cout << trad[p] << " " << line << " " << column << endl;
		column += strlen(yytext);
	}
	else if(p != LEXEOF){
		column += strlen(yytext);
	}
	return p;
}
  
int main(){
	while(nextToken() != LEXEOF);
}

