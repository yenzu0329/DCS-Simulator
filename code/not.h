#ifndef NOT_H
#define NOT_H

#include "gate.h"

class Not: public Gate
{
public:
    Not();
    Not(Gate *);
    Not(QStringList);
    void compute();
};

#endif // NOT_H
