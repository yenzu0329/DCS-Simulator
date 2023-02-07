#ifndef INLABEL_H
#define INLABEL_H

#include "label.h"

class InLabel: public Label
{
public:
    InLabel();
    InLabel(InLabel *);
    InLabel(QStringList);
    void setName(QString);
    ~InLabel();
    static int seq_num;
    static QVector<QString> names;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INLABEL_H
