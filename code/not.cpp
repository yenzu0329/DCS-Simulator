#include "not.h"

Not::Not()
{
    type = "NOT";
    InNode *in = inNodes.first();
    inNodes.pop_front();
    delete  in;
    pic.load(":/components/not/1.png");
    setNodesPos();
}

Not::Not(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

Not::Not(Gate *gate):Gate(gate)
{
    type = "NOT";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void Not::compute()
{
    Value ans = !inNodes[0]->getValue();
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
