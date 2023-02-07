#include "component.h"

int Component::seq_num = 0;
Component::Component():id(seq_num++)
{
    name = "id_" + QString::number(id);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    acceptedMouseButtons();
}

Component::Component(QStringList str):id(str[1].toInt())
{
    /*
     * 0: type
     * 1: id
     * 2: name
     * 3: pos_x
     * 4: pos_y
     * 5: innode_num
     * 6: outnode_num
     * else: [innodes_id], [outnodes_id], ...
     */
    if(id >= seq_num)    seq_num = id+1;
    type = str[0];
    name = str[2];
    setPos(QPointF(str[3].toFloat(), str[4].toFloat()));
    int ins = str[5].toInt();
    int outs = str[6].toInt();
    int i=7;
    for(; i<7+ins; i++)
    {
        InNode *in = new InNode(this, str[i].toInt());
        inNodes.push_back(in);
    }
    for(; i<7+ins+outs; i++)
    {
        OutNode *out = new OutNode(this, str[i].toInt());
        outNodes.push_back(out);
    }
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    acceptedMouseButtons();
}

Component::~Component()
{
    InNode *temp_in;
    OutNode *temp_out;

    while(!inNodes.isEmpty())
    {
        temp_in = inNodes.first();
        inNodes.pop_front();
        if(dynamic_cast<Wire *>(temp_in->getConnect()))
        {
            item_scene->removeItem(temp_in->getConnect());
            delete temp_in->getConnect();
        }
        else if(temp_in->getConnect())
            temp_in->setConnect(nullptr);
        else
            delete temp_in;
    }
    while(!outNodes.isEmpty())
    {
        temp_out = outNodes.first();
        outNodes.pop_front();
        if(dynamic_cast<Wire *>(temp_out->getConnect()))
        {
            item_scene->removeItem(temp_out->getConnect());
            delete temp_out->getConnect();
        }
        else if(temp_out->getConnect())
            temp_out->setConnect(nullptr);
        else
            delete temp_out;
    }
}

void Component::spreadSelect()
{
    if(isSelected())    return;
    setSelected(true);
    for (InNode *in: qAsConst(inNodes))
    {
        // wire to others
        if(dynamic_cast<FlipFlop *>(in->getBelong()))
            dynamic_cast<FlipFlop *>(in->getBelong())->spreadSelect();
        else
            in->getBelong()->spreadSelect();

        //others to wire
        if (in->getConnect())
            in->getConnect()->spreadSelect();
    }
    for (OutNode *out: qAsConst(outNodes))
    {
        // wire to others
        if(dynamic_cast<FlipFlop *>(out->getBelong()))
            dynamic_cast<FlipFlop *>(out->getBelong())->spreadSelect();
        else
            out->getBelong()->spreadSelect();

        //others to wire
        if (out->getConnect())
            out->getConnect()->spreadSelect();
    }
}

QString Component::getInfo()
{
    QString info;
    info = type + '$' + QString::number(id) + '$' + name + '$';
    info += QString::number(x()) + '$' + QString::number(y()) + '$';
    info += QString::number(inNodes.length()) + '$' + QString::number(outNodes.length()) + '$';
    for(InNode* in : qAsConst(inNodes))
        info += QString::number(in->getID()) + '$';
    for(OutNode* out : qAsConst(outNodes))
        info += QString::number(out->getID()) + '$';
    return info;
}

Node* Component::findNodeByID(int id)
{
    if(!inNodes.empty())
        for(InNode *in : qAsConst(inNodes))
            if(in->getID() == id)
                return in;
    if(!outNodes.empty())
        for(OutNode *out : qAsConst(outNodes))
            if(out->getID() == id)
                return out;
    return nullptr;
}
