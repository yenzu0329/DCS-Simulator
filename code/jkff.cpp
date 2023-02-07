#include "jkff.h"

JKff::JKff()
{
    type = "JKFF";
    InNode *in = new InNode(this);
    inNodes.push_back(in);

    pic.load(":/components/ff/jkff.png");
    setNodesPos();
}

JKff::JKff(QStringList str): FlipFlop(str)
{
    type = "JKFF";
    pic.load(":/components/ff/jkff.png");
    setNodesPos();
}

JKff::JKff(FlipFlop *ff): FlipFlop(ff)
{
    type = "JKFF";
    pic.load(":/components/ff/jkff.png");
    setNodesPos();
}

void JKff::compute()
{
    if(store_clk == ZERO && clock->getValue() == ONE)
    {
        if      (store_j==0 && store_k==0)  ;
        else if (store_j==0 && store_k==1)  outNodes[0]->setValue(0);
        else if (store_j==1 && store_k==0)  outNodes[0]->setValue(1);
        else if (store_j==1 && store_k==1)  outNodes[0]->setValue(!outNodes[0]->getValue());
        else                                outNodes[0]->setValue(UNKNOWN);
        outNodes[1]->setValue(!outNodes[0]->getValue());
    }
    store_j = inNodes[0]->getValue();
    store_k = inNodes[1]->getValue();
    FlipFlop::compute();
}

void JKff::init()
{
    FlipFlop::init();
    store_j = ZERO;
    store_k = ZERO;
}
