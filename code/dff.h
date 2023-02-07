#ifndef DFF_H
#define DFF_H

#include <flipflop.h>

class Dff: public FlipFlop
{
public:
    Dff();
    Dff(FlipFlop *);
    Dff(QStringList);
    void compute();
    void init();
private:
    Value store_d;
};

#endif // DFF_H
