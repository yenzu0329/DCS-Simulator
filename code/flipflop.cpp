#include "flipflop.h"

FlipFlop::FlipFlop()
{
    showName = false;
    //設定inNode & outNode
    InNode *in = new InNode(this);
    OutNode *out1 = new OutNode(this);
    OutNode *out2 = new OutNode(this);
    inNodes.push_back(in);
    outNodes.push_back(out1);
    outNodes.push_back(out2);
    clock = new InNode(this);
    setScale(0.1);
}

FlipFlop::FlipFlop(QStringList str): Component(str)
{
    showName = false;
    clock = new InNode(this, str.last().toInt());
    setScale(0.1);
}

FlipFlop::FlipFlop(FlipFlop *ff)
{
    showName = ff->showName;
    name = ff->name;
    item_scene = ff->item_scene;
    if(mouse.getMode() == NONE)
        setFlag(QGraphicsItem::ItemIsMovable, true);

    for(InNode *in : qAsConst(ff->inNodes))
    {
        in->setBelong(this);
        inNodes.push_back(in);
    }
    for(OutNode *out : qAsConst(ff->outNodes))
    {
        out->setBelong(this);
        outNodes.push_back(out);
    }
    clock = ff->clock;
    clock->setBelong(this);
    ff->clock = nullptr;

    while(!ff->inNodes.empty())
        ff->inNodes.pop_back();
    while(!ff->outNodes.empty())
        ff->outNodes.pop_back();
    setScale(0.1);
    setPos(ff->scenePos());
}

FlipFlop::~FlipFlop()
{
    if(clock)
    {
        if(dynamic_cast<Wire *>(clock->getConnect()))
        {
            item_scene->removeItem(clock->getConnect());
            delete clock->getConnect();
        }
        delete clock;
    }
}

void FlipFlop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont font;
    font.setPointSize(90);
    painter->setFont(font);

    painter->drawPixmap(-pic.width()/2, -pic.height()/2, pic);
    if(showName)
        painter->drawText(-pic.width()/2+150, -pic.height()/2-50, name);
    if(isSelected() && !mouse.temp_select)
    {
        QPixmap pixmap(":/components/ff/0_"+type.toLower()+".png");
        painter->drawPixmap(-pixmap.width()/2, -pixmap.height()/2, pixmap);
    }
}

QRectF FlipFlop::boundingRect() const
{
    return QRectF(-pic.width()/2, -pic.height()/2, pic.width(), pic.height());
}


void FlipFlop::setNodesPos()
{
    //draw inNodes
    for(int i=0; i<inNodes.length(); i++)
        inNodes[i]->setPos(-pic.width()/2, pic.height() * (-0.5 + (1.0+2*i)/4));
    clock->setPos(-pic.width()/2, 0);

    //draw outNodes
    outNodes[0]->setPos(+pic.width()/2, pic.height() * -0.25);
    outNodes[1]->setPos(+pic.width()/2, pic.height() * 0.25);
}

void FlipFlop::spreadSelect()
{
    Component::spreadSelect();
    if (clock->getConnect())
        clock->getConnect()->spreadSelect();
}

void FlipFlop::addInNode()
{
    InNode *in = new InNode(this);
    inNodes.push_back(in);
}

void FlipFlop::subInNode()
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
    update();
}

void FlipFlop::compute()
{
    store_clk = clock->getValue();
    if(outNodes[0]->getConnect())
        outNodes[0]->getConnect()->compute();
    if(outNodes[1]->getConnect())
        outNodes[1]->getConnect()->compute();
}

void FlipFlop::init()
{
    store_clk = ZERO;
    outNodes[1]->setValue(ONE);
    clock->setValue(ZERO);
}

QString FlipFlop::getInfo()
{
    QString info = Component::getInfo();
    info += QString::number(clock->getID());
    return info;
}

Node* FlipFlop::findNodeByID(int id)
{
    Node* node = Component::findNodeByID(id);
    if(clock->getID() == id)
        return clock;
    return node;
}
