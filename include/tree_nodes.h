#ifndef TREE_NODES
#define TREE_NODES

#include <deque>
#include <string>

#include "token.h"

using std::deque;
using std::string;

namespace ast{
	class expr;
	class variable;
	
	class position{
	private:
		int line;
		int column;
	public:
		position(int _line, int _column);
		int get_line() const;
		int get_column() const;
	};
	
	class token{
	private:
		yytokentype id;
		position pos;
	public:
		token(int _id, position _pos);
	};

	class stmt{
	protected:
		int line;
	public:
		stmt();
		void set_line(int _line);
	};

	class end : public stmt{ };

	class program{
	private:
		deque<stmt*> stmts;
	public:
		program();
		void push_front(stmt* _stmt);
	};
		
	class let_stmt : public stmt{
	private:
		variable* var;
		expr* val;
	public:
		let_stmt(variable* _var, expr* _val);
	};

	class expr{
	protected:
		token tok;
	public:
		expr(token _tok);
	};

	class binary_expr : public expr {
	private:
		expr* left;
		expr* right;
	public:
		binary_expr(expr* _left, token _tok, expr* _right);
	};
	
	class unary_expr : public expr {
	private:
		expr* target;
	public:
		unary_expr(token _tok, expr* _target);
	};
	
	class function_expr : public expr {
	private:
		string* name;
		expr* param;
	public:
		function_expr(token _tok, string* _name, expr* _param);
	};
	
	class variable : public expr {
	private:
		string* name;
		expr* idx1;
		expr* idx2;
	public:
		variable(token _tok, string* _name);
		variable(token _tok, string* _name, expr* _idx);
		variable(token _tok, string* _name, expr* _idx1, expr* _idx2);
	};
	
	template<class T>
	class literal_expr : public expr {
	private:
		T value;
	public:
		literal_expr(token _tok, T _value);
	};
	
}

#endif
