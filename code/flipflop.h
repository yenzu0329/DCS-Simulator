#ifndef FLIPFLOP_H
#define FLIPFLOP_H

#include "component.h"

class FlipFlop: public Component
{
public:
    FlipFlop();
    FlipFlop(FlipFlop *);
    FlipFlop(QStringList);
    ~FlipFlop();
    void setNodesPos();
    void addInNode();
    void subInNode();
    void spreadSelect();
    Node* findNodeByID(int);
    void setShowName(bool b){showName = b;}
    bool getShowName(){return showName;}
    InNode* getClock(){return clock;}
    virtual void init();
    QString getInfo() override;
    void compute() override;

protected:
    QPixmap pic;
    Value store_clk;
    InNode *clock;
    bool showName;

protected slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
};

#endif // FLIPFLOP_H
