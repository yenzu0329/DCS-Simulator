#ifndef OR_H
#define OR_H

#include "gate.h"

class Or: public Gate
{
public:
    Or();
    Or(Gate *);
    Or(QStringList);
    void compute();
};

#endif // OR_H
