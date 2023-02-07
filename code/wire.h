#ifndef WIRE_H
#define WIRE_H

#include <QGraphicsScene>
#include "component.h"

class Wire: public Component
{
public:
    Wire(Node *);
    Wire(QStringList, QStringList, QVector<void *>);
    void setHover();
    void addEndPoint();                 // for mousePressEvent
    void changeLastEndPoint(QPointF);   // for mouseMoveEvent
    bool deleteLastEndPoint();
    bool connectNode(Node *);
    bool connectWire(Wire *);
    void fixPos();
    QPointF fixNode(QPointF, Wire*);
    void reshape();
    QPointF getPos();
    bool connect(Node *);
    void compute() override;
    QString getInfo() override;

private:
    QVector<QPointF> endPoints;
    QVector<void *> check;
    bool hover;

private slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
};

#endif // WIRE_H
