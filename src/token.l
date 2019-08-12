%{ 

#include "token.h"

int line = 1, next_line = 1, column = 1, next_column = 1;
char * text;

%}
  
/* Rules Section*/
%option caseless
%% 
[ \t\r]								{return WHITE;}
REM[ ](.*)							{return COMMENT;}
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
STOP								{return STOP;}
THEN								{return THEN;}
TRUE								{return BOOLEAN;}
FALSE								{return BOOLEAN;}

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
\%									{return MOD;}
\^									{return EXPONENTIAL;}
>									{return GT;}
\<									{return LT;}
>=									{return GTE;}
\<=									{return LTE;}
=									{return EQUALS;}
\(									{return LPAREN;}
\)									{return RPAREN;}
,									{return COMMA;}
;									{return SEMICOLON;}

FN[A-Z]								{return FUNCTION;}
[A-Z]([0-9]?)						{return VARIABLE;}

[0-9]*\.[0-9]+						{return FLOAT;}
[0-9]+								{return INTEGER;}
\"[^\"\\]*(\\.[^\"\\]*)*\"			{return STRING;}
\'(\\n|\\t|\\r|\\0|\\'|\\\\|.)\'	{return CHAR;}

[A-Z0-9]+							{return LEXERROR;}
.									{return LEXERROR;}
<<EOF>>								{return LEXEOF;}

%% 

int yywrap(void){
	return 1;
}

token next_token(){
	token tok = (token) yylex();
	line = next_line;
	column = next_column;
	text = yytext;
	if(tok == ENDL){
		next_line++;
		next_column = 1;
	}
	else if(tok != LEXEOF){
		next_column += (int) strlen(yytext);
	}
	return tok;
}

