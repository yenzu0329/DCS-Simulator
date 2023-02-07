#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QVector>
#include "innode.h"
#include "outnode.h"

class Component: public QGraphicsItem
{
public:
    Component();
    Component(QStringList);
    ~Component();
    void setName(QString n){name = n;}
    void setType(QString t){type = t;}
    void setScene(QGraphicsScene *s){item_scene = s;}

    int getID(){return id;}
    QString getName(){return name;}
    QString getType(){return type;}
    QVector<InNode*>  getInNodes(){return inNodes;}
    QVector<OutNode*> getOutNodes(){return outNodes;}
    Node* findNodeByID(int);
    void spreadSelect();
    virtual void compute(){}
    virtual QString getInfo();
    static int seq_num;

protected:
    const  int id;
    QString type;
    QString name;
    QVector<InNode*> inNodes;
    QVector<OutNode*> outNodes;
    QGraphicsScene *item_scene;
};

#include "wire.h"
#include "flipflop.h"
#endif // COMPONENT_H
