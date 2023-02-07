#include "rsff.h"

RSff::RSff()
{
    type = "RSFF";
    InNode *in = new InNode(this);
    inNodes.push_back(in);

    pic.load(":/components/ff/rsff.png");
    setNodesPos();
}

RSff::RSff(QStringList str): FlipFlop(str)
{
    type = "RSFF";
    pic.load(":/components/ff/rsff.png");
    setNodesPos();
}

RSff::RSff(FlipFlop *ff): FlipFlop(ff)
{
    type = "RSFF";
    pic.load(":/components/ff/rsff.png");
    setNodesPos();
}

void RSff::compute()
{
    if(store_clk == ZERO && clock->getValue() == ONE)
    {
        if      (store_r==0 && store_s==0)  ;
        else if (store_r==1 && store_s==0)  outNodes[0]->setValue(0);
        else if (store_r==0 && store_s==1)  outNodes[0]->setValue(1);
        else                                outNodes[0]->setValue(UNKNOWN);
        outNodes[1]->setValue(!outNodes[0]->getValue());
    }
    store_r = inNodes[0]->getValue();
    store_s = inNodes[1]->getValue();
    FlipFlop::compute();
}

void RSff::init()
{
    FlipFlop::init();
    store_r = ZERO;
    store_s = ZERO;
}
