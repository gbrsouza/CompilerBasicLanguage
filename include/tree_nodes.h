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
		friend class visitor;
	public:
		position(int _line, int _column);
		int get_line() const;
		int get_column() const;
		void accept(const visitor& v) const;
	};
	
	class token{
	private:
		yytokentype id;
		position pos;
		friend class visitor;
	public:
		token(int _id, position _pos);
		void accept(const visitor& v) const;
	};

	class stmt{
	protected:
		token tok;
		int line;
		stmt(token _tok);
		friend class visitor;
	public:
		virtual ~stmt() = 0;
		void set_line(int _line);
		virtual void accept(const visitor& v) const = 0;
	};

	class program{
	private:
		deque<stmt*> stmts;
		friend class visitor;
	public:
		program();
		~program();
		void push_front(stmt* _stmt);
		void accept(const visitor& v) const;
	};

	class end_stmt : public stmt{
	private:
		friend class visitor;
	public:
		end_stmt(token _tok);
		void accept(const visitor& v) const override;
	};
		
	class let_stmt : public stmt{
	private:
		variable* var;
		expr* val;
		friend class visitor;
	public:
		let_stmt(token _tok, variable* _var, expr* _val);
		~let_stmt();
		void accept(const visitor& v) const override;
	};
	
	class print_stmt : public stmt{
	private:
		vector<print_expr>* values;
		friend class visitor;
	public:
		print_stmt(token _tok, vector<print_expr>* _values);
		~print_stmt();
		void accept(const visitor& v) const override;
	};
	
	class read_stmt : public stmt{
	private:
		vector<variable*>* var_list;
		friend class visitor;
	public:
		read_stmt(token _tok, vector<variable*>* _var_list);
		~read_stmt();
		void accept(const visitor& v) const override;
	};
	
	class data_stmt : public stmt{
	private:
		vector<expr*>* num_list;
		friend class visitor;
	public:
		data_stmt(token _tok, vector<expr*>* _num_list);
		~data_stmt();
		void accept(const visitor& v) const override;
	};
	
	class goto_stmt : public stmt{
	private:
		int target_line;
		friend class visitor;
	public:
		goto_stmt(token _tok, int _target_line);
		void accept(const visitor& v) const override;
	};
	
	class if_stmt : public stmt{
	private:
		expr* condition;
		int target_line;
		friend class visitor;
	public:
		if_stmt(token _tok, expr* _condition, int _target_line);
		~if_stmt();
		void accept(const visitor& v) const override;
	};
	
	class gosub_stmt : public stmt{
	private:
		int target_line;
		friend class visitor;
	public:
		gosub_stmt(token _tok, int _target_line);
		void accept(const visitor& v) const override;
	};
	
	class return_stmt : public stmt{
	private:
		friend class visitor;
	public:
		return_stmt(token _tok);
		void accept(const visitor& v) const override;
	};
	
	class def_stmt : public stmt{
	private:
		string* name;
		string* param;
		expr* target;
		friend class visitor;
	public:
		def_stmt(token _tok, string* _name, string* _param, expr* _target);
		~def_stmt();
		void accept(const visitor& v) const override;
	};
	
	class dim_stmt : public stmt{
	private:
		variable* var;
		friend class visitor;
	public:
		dim_stmt(token _tok, variable* _var);
		~dim_stmt();
		void accept(const visitor& v) const override;
	};
	
	class next_stmt : public stmt{
	private:
		variable* var;
		friend class visitor;
	public:
		next_stmt(token _tok, variable* _var);
		~next_stmt();
		void accept(const visitor& v) const override;
	};
	
	class for_stmt : public stmt{
	private:
		variable* var;
		expr* initial_value;
		expr* condition;
		expr* step;
		friend class visitor;
	public:
		for_stmt(token _tok, variable* _var, expr* _initial_value, expr* _condition, expr* _step = nullptr);
		~for_stmt();
		void accept(const visitor& v) const override;
	};
	
	class stop_stmt : public stmt{
	private:
		friend class visitor;
	public:
		stop_stmt(token _tok);
		void accept(const visitor& v) const override;
	};

	class expr{
	private:
		friend class visitor;
	protected:
		token tok;
		expr(token _tok);
	public:
		virtual ~expr() = 0;
		virtual void accept(const visitor& v) const = 0;
	};

	class binary_expr : public expr {
	private:
		expr* left;
		expr* right;
		friend class visitor;
	public:
		binary_expr(expr* _left, token _tok, expr* _right);
		~binary_expr();
		void accept(const visitor& v) const override;
	};
	
	class unary_expr : public expr {
	private:
		expr* target;
		friend class visitor;
	public:
		unary_expr(token _tok, expr* _target);
		~unary_expr();
		void accept(const visitor& v) const override;
	};
	
	class function_expr : public expr {
	private:
		string* name;
		expr* param;
		friend class visitor;
	public:
		function_expr(token _tok, string* _name, expr* _param);
		~function_expr();
		void accept(const visitor& v) const override;
	};
	
	class variable : public expr {
	private:
		string* name;
		expr* idx1;
		expr* idx2;
		friend class visitor;
	public:
		variable(token _tok, string* _name, expr* _idx1 = nullptr, expr* _idx2 = nullptr);
		~variable();
		void accept(const visitor& v) const override;
	};
	
	template<class T>
	class literal_expr : public expr {
	private:
		T value;
		friend class visitor;
	public:
		literal_expr(token _tok, T _value);
		~literal_expr();
		void accept(const visitor& v) const override;
	};
}

#endif
