#ifndef NODE_H
#define NODE_H

#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QGraphicsItem>
#include "mouse.h"
#include "value.h"

class Component;

class Node: public QGraphicsItem
{
public:
    Node(Component *);
    Node(Component *, int);
    ~Node();
    void setValue(ValueType);
    void setValue(Value);
    void setConnect(Component *);
    void setBelong(Component *);
    void setHover();
    Value      getValue();
    Component* getConnect();    // wire
    Component* getBelong();     // gate, flipflop, inlabel, outlabel
    int getID();
    static int seq_num;

protected:
    const int id;
    Value value;
    bool hover;
    Component *connect;
    Component *belong;

private slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
};

#endif // NODE_H
