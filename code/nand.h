#ifndef NAND_H
#define NAND_H

#include "gate.h"

class Nand: public Gate
{
public:
    Nand();
    Nand(Gate *);
    Nand(QStringList);
    void compute();
};

#endif // NAND_H
