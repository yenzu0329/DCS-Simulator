#ifndef AND_H
#define AND_H

#include "gate.h"

class And: public Gate
{
public:
    And();
    And(Gate *);
    And(QStringList);
    void compute();
};

#endif // AND_H
