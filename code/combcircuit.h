#ifndef COMBCIRCUIT_H
#define COMBCIRCUIT_H

#include "circuit.h"

class CombCircuit: public Circuit
{
public:
    CombCircuit();
    void comb2seq();
};

#endif // COMBCIRCUIT_H
