#include "tree_nodes.h"

using namespace ast;
	
position::position(int _line, int _column) : line(_line), column(_column) { }

int position::get_line() const {
	return line;
}

int position::get_column() const {
	return column;
}

token::token(/*yytokentype _id, */position _pos) : /*id(_id), */pos(_pos) { }

stmt::stmt() : line(-1) { }

void stmt::set_line(int _line){
	line = _line;
}

program::program() : stmts{} { }

void program::push_front(stmt* stmt){
	stmts.push_front(stmt);
}

let_stmt::let_stmt(variable* _var, expr* _val) : var(_var), val(_val) { }

expr::expr(token _tok) : tok(_tok) { }

binary_expr::binary_expr(expr* _left, token _tok, expr* _right) : expr(_tok), left(_left), right(_right) { }

unary_expr::unary_expr(token _tok, expr* _target) : expr(_tok), target(_target) { }

function_expr::function_expr(token _tok, const string& _name, expr* _param) : expr(_tok), name(_name), param(_param) { }

variable::variable(token _tok, const string& _name) : variable(_tok, _name, nullptr, nullptr) { }

variable::variable(token _tok, const string& _name, expr* _idx) : variable(_tok, _name, _idx, nullptr) { }

variable::variable(token _tok, const string& _name, expr* _idx1, expr* _idx2) : expr(_tok), name(_name), idx1(_idx1), idx2(_idx2) { }

template<class T>
literal_expr<T>::literal_expr(token _tok, T _value) : expr(_tok), value(_value) { }
