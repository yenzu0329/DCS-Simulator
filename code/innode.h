#ifndef INNODE_H
#define INNODE_H

#include "node.h"

class InNode: public Node
{
public:
    InNode(Component*);
    InNode(Component*, int);
};

#endif // INNODE_H
