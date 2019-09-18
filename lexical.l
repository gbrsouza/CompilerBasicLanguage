%{ 

#include<string.h> 
int i = 0; 
%} 
  
/* Rules Section*/
%% 
" "                 ;
LET                 {printf("letToken ");} 
PRINT               {printf("printToken ");}
END                 {printf("endToken ");}
READ                {printf("readToken ");}
DATA                {printf("dataToken ");}
GOTO                {printf("gotoToken ");}
IF				    {printf("ifToken ");}
FOR                 {printf("forToken ");}
NEXT                {printf("nextToken ");}
GOSUB               {printf("gosubToken ");}
RETURN              {printf("returnToken ");}
DEF                 {printf("defToken ");}
DIM                 {printf("dimToken ");}
REM                 {printf("commentToken ");}
STOP                {printf("stopToken ");}
THEN				{printf("thenToken ");}
TO					{printf("toToken ");}
STEP                {printf("stepToken ");}

ABS                 {printf("absToken ");}
ATN                 {printf("atnToken ");}
COS                 {printf("cosToken ");}
EXP                 {printf("expToken ");}
INT                 {printf("intToken ");}
LOG                 {printf("logToken ");}
RND                 {printf("rndToken ");}
SIN                 {printf("sinToken ");}
SQR                 {printf("sqrToken ");}
TAN                 {printf("tanToken ");}

\+					{printf("plusToken ");}
\-					{printf("minusToken ");}
\*                  {printf("timesToken ");}
\/                  {printf("divideToken ");}
\^                  {printf("exponentialToken ");}
>					{printf("gtToken ");}
\<					{printf("ltToken ");}
>=					{printf("gteToken ");}
\<=					{printf("lteToken ");}
=                   {printf("equalsToken ");}
\(                  {printf("lParenToken ");}
\)                  {printf("rParenToken ");}

FN[0-9]             {printf("functionToken ");}
[A-Z]([0-9]?)       {printf("variableToken ");}

[0-9]*              {printf("integerToken ");}

%% 
  
int yywrap(void){} 
  
int main() 
{    
    // The function that starts the analysis 
    yylex(); 
} 
