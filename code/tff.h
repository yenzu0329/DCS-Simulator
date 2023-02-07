#ifndef TFF_H
#define TFF_H

#include <flipflop.h>

class Tff: public FlipFlop
{
public:
    Tff();
    Tff(FlipFlop *);
    Tff(QStringList);
    void compute();
    void init();
private:
    Value store_t;
};

#endif // TFF_H
