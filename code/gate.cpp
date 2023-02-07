#include "gate.h"

Gate::Gate()
{
    showName = false;
    //設定inNode & outNode
    InNode *in1 = new InNode(this);
    InNode *in2 = new InNode(this);
    OutNode *out = new OutNode(this);
    inNodes.push_back(in1);
    inNodes.push_back(in2);
    outNodes.push_back(out);
    setScale(0.1);
}

Gate::Gate(QStringList str):Component(str)
{
    showName = false;
    setScale(0.1);
}

Gate::Gate(Gate *gate)  //copy and delete the old one
{
    showName = gate->showName;
    name = gate->name;
    item_scene = gate->item_scene;
    if(mouse.getMode() == NONE)
        setFlag(QGraphicsItem::ItemIsMovable, true);

    for(InNode *in : qAsConst(gate->inNodes))
    {
        in->setBelong(this);
        inNodes.push_back(in);
    }
    for(OutNode *out : qAsConst(gate->outNodes))
    {
        out->setBelong(this);
        outNodes.push_back(out);
    }

    while(!gate->inNodes.empty())
        gate->inNodes.pop_back();
    while(!gate->outNodes.empty())
        gate->outNodes.pop_back();
    setScale(0.1);
    setPos(gate->scenePos());
}

void Gate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont font;
    font.setPointSize(90);
    painter->setFont(font);

    painter->drawPixmap(-pic.width()/2, -pic.height()/2, pic);
    if(showName)
        painter->drawText(-pic.width()/2+100, -pic.height()/2-50, name);
    if(isSelected() && !mouse.temp_select)
    {
        QPixmap pixmap(":/components/"+type.toLower()+"/0.png");
        painter->drawPixmap(-pixmap.width()/2, -pixmap.height()/2, pixmap);
    }
}

QRectF Gate::boundingRect() const
{
    return QRectF(-pic.width()/2, -pic.height()/2, pic.width(), pic.height());
}

void Gate::setNodesPos()
{
    //draw inNodes
    for(int i=0; i<inNodes.length(); i++)
        inNodes[i]->setPos(-pic.width()/2, pic.height() * (-0.5 + (1.0+2*i)/(2*inNodes.length())));
    //draw outNodes
    outNodes[0]->setPos(pic.width()/2, 0);
}

void Gate::addInNode()
{
    InNode *in = new InNode(this);
    inNodes.push_back(in);
    if(pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png"))
        setNodesPos();
    for(InNode *in : qAsConst(inNodes))
    {
        if(in->getConnect())
            dynamic_cast<Wire *>(in->getConnect())->reshape();
    }
    update();
}

void Gate::subInNode()
{
    InNode *temp_in;
    temp_in = inNodes.last();
    inNodes.pop_back();

    if(dynamic_cast<Wire *>(temp_in->getConnect()))
    {
        item_scene->removeItem(temp_in->getConnect());
        delete temp_in->getConnect();
    }
    delete temp_in;
    if(pic.load(":/components/"+ type.toLower() +"/"+ QString::number(inNodes.length()) +".png"))
        setNodesPos();
    for(InNode *in : qAsConst(inNodes))
    {
        if(in->getConnect())
            dynamic_cast<Wire *>(in->getConnect())->reshape();
    }
    update();
}
