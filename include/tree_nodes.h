#ifndef TREE_NODES
#define TREE_NODES

#include <deque>
#include <string>
#include <vector>
#include <utility>

#include "token.h"

#include "visitor.h"

using std::deque;
using std::string;
using std::vector;

namespace ast{
	class expr;
	class variable;
	
	typedef std::pair<expr*, bool> print_expr;
	
	class position{
	private:
		int line;
		int column;
		friend void accept ( visitor& v);

	public:
		position(int _line, int _column);
		int get_line() const;
		int get_column() const;
	};
	
	class token{
	private:
		yytokentype id;
		position pos;
		friend void accept ( visitor& v);
	public:
		token(int _id, position _pos);
	};

	class stmt{
	protected:
		token tok;
		int line;
		stmt(token _tok);
	public:
		void set_line(int _line);
	};

	class program{
	private:
		deque<stmt*> stmts;
	public:
		program();
		void push_front(stmt* _stmt);
		~program();
	};

	class end_stmt : public stmt{
	public:
		end_stmt(token _tok);
	};
		
	class let_stmt : public stmt{
	private:
		variable* var;
		expr* val;
	public:
		let_stmt(token _tok, variable* _var, expr* _val);
		~let_stmt();
	};
	
	class print_stmt : public stmt{
	private:
		vector<print_expr>* values;
	public:
		print_stmt(token _tok, vector<print_expr>* _values);
		~print_stmt();
	};
	
	class read_stmt : public stmt{
	private:
		vector<variable*>* var_list;
	public:
		read_stmt(token _tok, vector<variable*>* _var_list);
		~read_stmt();
	};
	
	class data_stmt : public stmt{
	private:
		vector<expr*>* num_list;
	public:
		data_stmt(token _tok, vector<expr*>* _num_list);
		~data_stmt();
	};
	
	class goto_stmt : public stmt{
	private:
		int target_line;
	public:
		goto_stmt(token _tok, int _target_line);
	};
	
	class if_stmt : public stmt{
	private:
		expr* condition;
		int target_line;
	public:
		if_stmt(token _tok, expr* _condition, int _target_line);
		~if_stmt();
	};
	
	class gosub_stmt : public stmt{
	private:
		int target_line;
	public:
		gosub_stmt(token _tok, int _target_line);
	};
	
	class return_stmt : public stmt{
	public:
		return_stmt(token _tok);
	};
	
	class def_stmt : public stmt{
	private:
		string* name;
		string* param;
		expr* target;
	public:
		def_stmt(token _tok, string* _name, string* _param, expr* _target);
		~def_stmt();
	};
	
	class dim_stmt : public stmt{
	private:
		variable* var;
	public:
		dim_stmt(token _tok, variable* _var);
		~dim_stmt();
	};
	
	class next_stmt : public stmt{
	private:
		variable* var;
	public:
		next_stmt(token _tok, variable* _var);
		~next_stmt();
	};
	
	class for_stmt : public stmt{
	private:
		variable* var;
		expr* initial_value;
		expr* condition;
		expr* step;
	public:
		for_stmt(token _tok, variable* _var, expr* _initial_value, expr* _condition, expr* _step = nullptr);
		~for_stmt();
	};
	
	class stop_stmt : public stmt{
	public:
		stop_stmt(token _tok);
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
		~binary_expr();
	};
	
	class unary_expr : public expr {
	private:
		expr* target;
	public:
		unary_expr(token _tok, expr* _target);
		~unary_expr();
	};
	
	class function_expr : public expr {
	private:
		string* name;
		expr* param;
	public:
		function_expr(token _tok, string* _name, expr* _param);
		~function_expr();
	};
	
	class variable : public expr {
	private:
		string* name;
		expr* idx1;
		expr* idx2;
	public:
		variable(token _tok, string* _name, expr* _idx1 = nullptr, expr* _idx2 = nullptr);
		~variable();
	};
	
	template<class T>
	class literal_expr : public expr {
	private:
		T value;
	public:
		literal_expr(token _tok, T _value);
		~literal_expr();
	};
	
}

#endif
