#ifndef XOR_H
#define XOR_H

#include "gate.h"

class Xor: public Gate
{
public:
    Xor();
    Xor(Gate *);
    Xor(QStringList);
    void compute();
};

#endif // XOR_H
