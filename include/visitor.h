#ifndef VISITOR_H
#define VISITOR_H

#include "tree_nodes.h"
using namespace ast;


class visitor {

public:
	void visit ( position * ); 
	void visit ( token * );
	void visit ( end * );
	void visit ( program * ); 
	void visit ( let_stmt * );
	void visit ( binary_expr * );
	void visit ( unary_expr * );
	void visit ( function_expr * );
	void visit ( variable * );
	void visit ( unary_expr * );
};

#endif
