%{ 

#include <iostream>
#include <stdlib.h>
#include <string>

#include "return_types.hpp"
#include "token.hpp"

using std::string;

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line; \
    yylloc.first_column = yylloc.last_column; \
    for(char* c = yytext; *c; c++) { \
        if(*c == '\n') { \
            yylloc.last_line++; \
            yylloc.last_column = 1; \
        } \
        else { \
            yylloc.last_column++; \
        } \
    }
%}

/* Rules Section*/
%option caseless
%% 
[ \t\r]								{/*return WHITE;*/}
REM[ ](.*)							{/*return COMMENT;*/}
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
TRUE								{yylval._bool = true; return BOOLEAN;}
FALSE								{yylval._bool = false; return BOOLEAN;}

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

FN[A-Z]								{yylval._name = new std::string(yytext); return FUNCTION;}
[A-Z]([0-9]?)						{yylval._name = new std::string(yytext); return VARIABLE;}

[0-9]*\.[0-9]+						{yylval._name = new std::string(yytext); return FLOAT;}
[0-9]+								{yylval._int = atoi(yytext); return INTEGER;}
\"[^\"\\]*(\\.[^\"\\]*)*\"			{yylval._name = new std::string(yytext); return STRING;}
\'(\\n|\\t|\\r|\\0|\\'|\\\\|.)\'	{yylval._name = new std::string(yytext); return CHAR;}

[A-Z0-9]+							{return LEXERROR;}
.									{return LEXERROR;}
<<EOF>>								{return LEXEOF;}

%% 

int yywrap(void){
	return 1;
}

