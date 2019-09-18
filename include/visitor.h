#ifndef VISITOR_H
#define VISITOR_H

class visitor {

public:
	virtual void visit_position ( position * ) = 0; 
};

#endif
