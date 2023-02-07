#ifndef SELECTBOX_H
#define SELECTBOX_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>

class SelectBox: public QGraphicsRectItem
{
public:
    SelectBox();
    SelectBox(QPointF);
    ~SelectBox();
    void setStartPos(QPointF);
    void setEndPos(QPointF);
    void fixPos();
    void reshape();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    QRectF *box;
    QPen *pen;
    QPointF startPos;
};

#endif // SELECTBOX_H
