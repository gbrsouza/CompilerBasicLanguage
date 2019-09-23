#include "tree_nodes.h"

using namespace ast;
	
position::position(int _line, int _column) : line(_line), column(_column) { }

int position::get_line() const {
	return line;
}

int position::get_column() const {
	return column;
}

void position::accept(const visitor&v) const{
	v.visit(*this);
}

token::token(int _id, position _pos) : id((yytokentype)_id), pos(_pos) { }

void token::accept(const visitor&v) const{
	v.visit(*this);
}

stmt::stmt(token _tok) : tok(_tok), line(-1) { }

stmt::~stmt(){ }

void stmt::set_line(int _line){
	line = _line;
}

program::program() : stmts{} { }

void program::accept(const visitor&v) const{
	v.visit(*this);
}

void program::push_front(stmt* stmt){
	stmts.push_front(stmt);
}

program::~program(){
	for(stmt* _stmt : stmts){
		delete _stmt;
	}
}

end_stmt::end_stmt(token _tok) : stmt(_tok) { }

void end_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

let_stmt::let_stmt(token _tok, variable* _var, expr* _val) : stmt(_tok), var(_var), val(_val) { }

let_stmt::~let_stmt(){
	delete var;
	delete val;
}

void let_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

print_stmt::print_stmt(token _tok, vector<print_expr>* _values) : stmt(_tok), values(_values) { }

print_stmt::~print_stmt(){
	for(print_expr value : *values){
		delete value.first;
	}
	delete values;
}

void print_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

read_stmt::read_stmt(token _tok, vector<variable*>* _var_list) : stmt(_tok), var_list(_var_list) { }

read_stmt::~read_stmt(){
	for(variable* var : *var_list){
		delete var;
	}
	delete var_list;
}

void read_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

data_stmt::data_stmt(token _tok, vector<expr*>* _num_list) : stmt(_tok), num_list(_num_list) { }

data_stmt::~data_stmt(){
	for(expr* num : *num_list){
		delete num;
	}
	delete num_list;
}

void data_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

goto_stmt::goto_stmt(token _tok, int _target_line) : stmt(_tok), target_line(_target_line) { }

void goto_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

if_stmt::if_stmt(token _tok, expr* _condition, int _target_line) : stmt(_tok), condition(_condition), target_line(_target_line) { }

if_stmt::~if_stmt(){
	delete condition;
}

void if_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

gosub_stmt::gosub_stmt(token _tok, int _target_line) : stmt(_tok), target_line(_target_line) { }

void gosub_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

return_stmt::return_stmt(token _tok) : stmt(_tok) { }

void return_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

def_stmt::def_stmt(token _tok, string* _name, string* _param, expr* _target) : stmt(_tok), name(_name), param(_param), target(_target) { }

def_stmt::~def_stmt(){
	delete name;
	delete param;
	delete target;
}

void def_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

dim_stmt::dim_stmt(token _tok, variable* _var) : stmt(_tok), var(_var) { }

dim_stmt::~dim_stmt(){
	delete var;
}

void dim_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

next_stmt::next_stmt(token _tok, variable* _var) : stmt(_tok), var(_var) { }

next_stmt::~next_stmt(){
	delete var;
}

void next_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

for_stmt::for_stmt(token _tok, variable* _var, expr* _initial_value, expr* _condition, expr* _step) : stmt(_tok), var(_var), initial_value(_initial_value), condition(_condition), step(_step) { }

for_stmt::~for_stmt(){
	delete var;
	delete initial_value;
	delete condition;
	if(step != nullptr){
		delete step;
	}
}

void for_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

stop_stmt::stop_stmt(token _tok) : stmt(_tok) { }

void stop_stmt::accept(const visitor&v) const{
	v.visit(*this);
}

expr::expr(token _tok) : tok(_tok) { }

expr::~expr(){ }

binary_expr::binary_expr(expr* _left, token _tok, expr* _right) : expr(_tok), left(_left), right(_right) { }

binary_expr::~binary_expr(){
	delete left;
	delete right;
}

void binary_expr::accept(const visitor&v) const{
	v.visit(*this);
}

unary_expr::unary_expr(token _tok, expr* _target) : expr(_tok), target(_target) { }

unary_expr::~unary_expr(){
	delete target;
}

void unary_expr::accept(const visitor&v) const{
	v.visit(*this);
}

function_expr::function_expr(token _tok, string* _name, expr* _param) : expr(_tok), name(_name), param(_param) { }

function_expr::~function_expr(){
	delete name;
	delete param;
}

void function_expr::accept(const visitor&v) const{
	v.visit(*this);
}

variable::variable(token _tok, string* _name, expr* _idx1, expr* _idx2) : expr(_tok), name(_name), idx1(_idx1), idx2(_idx2) { }

variable::~variable(){
	delete name;
	if(idx1 != nullptr){
		delete idx1;
	}
	if(idx2 != nullptr){
		delete idx2;
	}
}

void variable::accept(const visitor&v) const{
	v.visit(*this);
}

template<class T>
literal_expr<T>::literal_expr(token _tok, T _value) : expr(_tok), value(_value) { }

template<class T>
literal_expr<T>::~literal_expr(){ }

template<class T>
void literal_expr<T>::accept(const visitor&v) const{
	v.visit(*this);
}

namespace ast{
	template<>
	literal_expr<string*>::~literal_expr(){
		delete value;
	}
}

template class ast::literal_expr<string*>;
template class ast::literal_expr<int>;
template class ast::literal_expr<bool>;


