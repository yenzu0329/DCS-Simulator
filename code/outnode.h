#ifndef OUTNODE_H
#define OUTNODE_H

#include "node.h"

class OutNode: public Node
{
public:
    OutNode(Component*);
    OutNode(Component*, int);
};


#endif // OUTNODE_H
