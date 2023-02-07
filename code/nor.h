#ifndef NOR_H
#define NOR_H

#include "gate.h"

class Nor: public Gate
{
public:
    Nor();
    Nor(Gate *);
    Nor(QStringList);
    void compute();
};

#endif // NOR_H
