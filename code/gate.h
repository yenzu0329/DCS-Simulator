#ifndef GATE_H
#define GATE_H

#include "component.h"

class Gate: public Component
{
public:
    Gate();
    Gate(Gate *);
    Gate(QStringList);
    void addInNode();
    void subInNode();
    void setNodesPos();
    void setShowName(bool b){showName = b;}
    bool getShowName(){return showName;}

protected:
    QPixmap pic;
    bool showName;

protected slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
};

#endif // GATE_H
