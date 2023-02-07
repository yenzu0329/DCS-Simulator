#include "nor.h"

Nor::Nor()
{
    type = "NOR";
    pic.load(":/components/nor/2.png");
    setNodesPos();
}

Nor::Nor(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

Nor::Nor(Gate *gate):Gate(gate)
{
    type = "NOR";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void Nor::compute()
{
    Value ans = inNodes[0]->getValue();
    for(int i=1 ; i < inNodes.length() ; i++)
        ans |= inNodes[i]->getValue();
    ans = !ans;
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
