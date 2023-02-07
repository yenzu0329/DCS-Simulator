#include "nand.h"

Nand::Nand()
{
    type = "NAND";
    pic.load(":/components/nand/2.png");
    setNodesPos();
}

Nand::Nand(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

Nand::Nand(Gate *gate):Gate(gate)
{
    type = "NAND";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void Nand::compute()
{
    Value ans = inNodes[0]->getValue();
    for(int i=1 ; i < inNodes.length() ; i++)
        ans &= inNodes[i]->getValue();
    ans = !ans;
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
