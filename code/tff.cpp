#include "tff.h"

Tff::Tff()
{
    type = "TFF";
    pic.load(":/components/ff/tff.png");
    setNodesPos();
}

Tff::Tff(QStringList str): FlipFlop(str)
{
    type = "TFF";
    pic.load(":/components/ff/tff.png");
    setNodesPos();
}

Tff::Tff(FlipFlop *ff): FlipFlop(ff)
{
    type = "TFF";
    pic.load(":/components/ff/tff.png");
    setNodesPos();
}

void Tff::compute()
{
    if(store_clk == ZERO && clock->getValue() == ONE)
    {
        if      (store_t != outNodes[0]->getValue())    outNodes[0]->setValue(1);
        else if (store_t == outNodes[0]->getValue())    outNodes[0]->setValue(0);
        else                outNodes[0]->setValue(UNKNOWN);
        outNodes[1]->setValue(!outNodes[0]->getValue());
    }
    store_t = inNodes[0]->getValue();
    FlipFlop::compute();
}

void Tff::init()
{
    FlipFlop::init();
    store_t = ZERO;
}
