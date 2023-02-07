#include "and.h"

And::And()
{
    type = "AND";
    pic.load(":/components/and/2.png");
    setNodesPos();
}

And::And(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

And::And(Gate *gate):Gate(gate)
{
    type = "AND";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void And::compute()
{
    Value ans = inNodes[0]->getValue();
    for(int i=1 ; i < inNodes.length() ; i++)
        ans &= inNodes[i]->getValue();
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
