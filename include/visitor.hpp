#ifndef VISITOR_H
#define VISITOR_H

#include <string>

using std::string;

namespace ast{

class position;
class token;
class program;
class end_stmt;
class let_stmt;
class print_stmt;
class read_stmt;
class input_stmt;
class data_stmt;
class goto_stmt;
class if_stmt;
class gosub_stmt;
class return_stmt;
class def_stmt;
class dim_stmt;
class next_stmt;
class for_stmt;
class stop_stmt;
class binary_expr;
class unary_expr;
class function_expr;
class variable;
class expr;
template<class T> class literal_expr;

class visitor{
public:
	void visit(const position& node) const;
	void visit(const token& node) const;
	void visit(const program& node) const;
	void visit(const empty_stmt& node) const;
	void visit(const end_stmt& node) const;
	void visit(const let_stmt& node) const;
	void visit(const print_stmt& node) const;
	void visit(const read_stmt& node) const;
	void visit(const input_stmt& node) const;
	void visit(const data_stmt& node) const;
	void visit(const goto_stmt& node) const;
	void visit(const if_stmt& node) const;
	void visit(const gosub_stmt& node) const;
	void visit(const return_stmt& node) const;
	void visit(const def_stmt& node) const;
	void visit(const dim_stmt& node) const;
	void visit(const next_stmt& node) const;
	void visit(const for_stmt& node) const;
	void visit(const stop_stmt& node) const;
	void visit(const binary_expr& node) const;
	void visit(const unary_expr& node) const;
	void visit(const function_expr& node) const;
	void visit(const variable& node) const;
	template<class T> void visit(const literal_expr<T>& node) const;
	
};

extern template void visitor::visit<string*>(const literal_expr<string*>& node) const;
extern template void visitor::visit<int>(const literal_expr<int>& node) const;
extern template void visitor::visit<bool>(const literal_expr<bool>& node) const;

}

#endif
