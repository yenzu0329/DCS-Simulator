#include "or.h"

Or::Or()
{
    type = "OR";
    pic.load(":/components/or/2.png");
    setNodesPos();
}

Or::Or(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

Or::Or(Gate *gate):Gate(gate)
{
    type = "OR";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void Or::compute()
{
    Value ans = inNodes[0]->getValue();
    for(int i=1 ; i < inNodes.length() ; i++)
        ans |= inNodes[i]->getValue();
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
