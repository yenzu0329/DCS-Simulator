#include "dff.h"

Dff::Dff()
{
    type = "DFF";
    pic.load(":/components/ff/dff.png");
    setNodesPos();
}

Dff::Dff(QStringList str): FlipFlop(str)
{
    pic.load(":/components/ff/dff.png");
    setNodesPos();
}

Dff::Dff(FlipFlop *ff): FlipFlop(ff)
{
    type = "DFF";
    pic.load(":/components/ff/dff.png");
    setNodesPos();
}

void Dff::compute()
{
    if(store_clk == ZERO && clock->getValue() == ONE)
    {
        outNodes[0]->setValue(store_d);
        outNodes[1]->setValue(!store_d);
    }
    store_d = inNodes[0]->getValue();
    FlipFlop::compute();
}

void Dff::init()
{
    FlipFlop::init();
    store_d = ZERO;
}
