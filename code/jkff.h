#ifndef JKFF_H
#define JKFF_H

#include <flipflop.h>

class JKff: public FlipFlop
{
public:
    JKff();
    JKff(FlipFlop *);
    JKff(QStringList);
    void compute();
    void init();
private:
    Value store_j;
    Value store_k;
};

#endif // JKFF_H
