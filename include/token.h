#ifndef TOKEN
#define TOKEN

#include <initializer_list>

extern int line, column;
extern char * text;

enum token {
	ABS			=  1,
	AND			=  2,
	ATN			=  3,
	BOOLEAN		=  4,
	CHAR		=  5,
	COMMA		=  6,
	COMMENT		=  7,
	COS			=  8,
	DATA		=  9,
	DEF			= 10,
	DIFF		= 11,
	DIM			= 12,
	DIVIDE		= 13,
	END			= 14,
	ENDL		= 15,
	EQUALS		= 16,
	EXP			= 17,
	EXPONENTIAL	= 18,
	FLOAT		= 19,
	FOR			= 20,
	FUNCTION	= 21,
	GOSUB		= 22,
	GOTO		= 23,
	GT			= 24,
	GTE			= 25,
	IF			= 26,
	INPUT		= 27,
	INT			= 28,
	INTEGER		= 29,
	LET			= 30,
	LEXEOF		= 31,
	LEXERROR	= 32,
	LOG			= 33,
	LPAREN		= 34,
	LT			= 35,
	LTE			= 36,
	MINUS		= 37,
	MOD			= 38,
	NEXT		= 39,
	NOT			= 40,
	OR			= 41,
	PLUS		= 42,
	PRINT		= 43,
	READ		= 44,
	RETURN		= 45,
	RND			= 46,
	RPAREN		= 47,
	SEMICOLON	= 48,
	SIN			= 49,
	SQR			= 50,
	STEP		= 51,
	STOP		= 52,
	STRING		= 53,
	TAN			= 54,
	THEN		= 55,
	TIMES		= 56,
	TO			= 57,
	VARIABLE	= 58,
	WHITE		= 59,
};

constexpr std::initializer_list<token> all_tokens =
{
ABS,AND,ATN,BOOLEAN,CHAR,COMMA,COMMENT,COS,DATA,DEF,DIFF,DIM,DIVIDE,END,ENDL,EQUALS,EXP,EXPONENTIAL,FLOAT,FOR,FUNCTION,GOSUB,GOTO,GT,GTE,IF,
INPUT,INT,INTEGER,LET,LEXEOF,LEXERROR,LOG,LPAREN,LT,LTE,MINUS,MOD,NEXT,NOT,OR,PLUS,PRINT,READ,RETURN,RND,RPAREN,SEMICOLON,SIN,SQR,STEP,STOP,
STRING,TAN,THEN,TIMES,TO,VARIABLE,WHITE,
};

token next_token();

#endif
