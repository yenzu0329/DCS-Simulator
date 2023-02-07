#include "xor.h"

Xor::Xor()
{
    type = "XOR";
    pic.load(":/components/xor/2.png");
    setNodesPos();
}

Xor::Xor(QStringList str):Gate(str)
{
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

Xor::Xor(Gate *gate):Gate(gate)
{
    type = "XOR";
    pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png");
    setNodesPos();
}

void Xor::compute()
{
    Value ans = ZERO;
    for(int i=0 ; i < inNodes.length() ; i++)
        if(inNodes[i]->getValue().type() == ONE)
            ans = !ans;
        else if(inNodes[i]->getValue().type() == UNKNOWN)
            ans = UNKNOWN;
    outNodes[0]->setValue(ans);
    outNodes[0]->getConnect()->compute();
}
