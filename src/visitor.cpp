#include <iostream>

#include "visitor.h"

void visitor::visit ( end *node ) {
	std::cout << "{ \"command\": \"END\", \"line\" :  " 
			  << node->line 
			  << " }\n";
}

void visitor::visit ( position *node) {
	std::cout << "{ \"command\" : \"POSITION\", \"line\" : "
			  << node->line
			  << ", \"column\" :"
			  << node->column
			  << " }\n";
}

void visitor::visit ( token *node){
	std::cout << "{ \"command\" : \"TOKEN\", \"position\" : "
			  << this->visit(node->pos)
			  << " }\n";
}

void visitor::visit ( program *node) {}
void visitor::visit ( let_stmt *node) {}
void visitor::visit ( binary_expr *node){}
void visitor::visit ( unary_expr *node){}
void visitor::visit ( function_expr *node){}
void visitor::visit ( variable *node){}
void visitor::visit ( unary_expr *node){}
