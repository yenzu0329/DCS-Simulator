#ifndef LABEL_H
#define LABEL_H

#include "component.h"
#include "text.h"

class Label: public Component
{
public:
    Label();
    Label(Label *);
    Label(QStringList);
    ~Label();
    Text *text;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // LABEL_H
