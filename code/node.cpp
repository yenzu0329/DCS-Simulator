#include "node.h"

int Node::seq_num = 2;
Node::Node(Component* b):QGraphicsItem((QGraphicsItem*)b), id(seq_num++)
{
    belong = b;
    hover = false;
    connect = nullptr;
    setAcceptHoverEvents(true);
}
Node::Node(Component* b, int id):QGraphicsItem((QGraphicsItem*)b), id(id)
{
    if(id >= seq_num)    seq_num = id+1;
    belong = b;
    hover = false;
    connect = nullptr;
    setAcceptHoverEvents(true);
}

Node::~Node()
{
    belong = nullptr;
}

void  Node::setValue(ValueType v)   {value = v;}
void  Node::setValue(Value v)       {value = v;}
void  Node::setConnect(Component *c){connect = c;}
void  Node::setBelong(Component *b) {belong = b;    setParentItem((QGraphicsItem*)b);}
void  Node::setHover()              {hover = true;}
Value      Node::getValue()         {return value;}
Component* Node::getConnect()       {return connect;}
Component* Node::getBelong()        {return belong;}
int        Node::getID()            {return id;}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!connect)
    {
        if(hover)   painter->setPen(QPen(Qt::red, 10));
        else        painter->setPen(QPen(Qt::black, 10));
        painter->drawRect(-30, -30, 60, 60);
    }
    hover = false;
}

QRectF Node::boundingRect() const
{
    return QRectF(-30, -30, 60, 60);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if(mouse.getMode() == WIRE && !connect)
        mouse.setNode(this);
}
