#ifndef COMBIN_H
#define COMBIN_H

#include "inlabel.h"
class SeqIn;

class CombIn: public InLabel
{
public:
    CombIn();
    CombIn(SeqIn *);
    CombIn(QStringList);
    void compute() override;
    void setValue(int i){value = i;}
    void setValueForCompute(Value b);
    Value getValue(){return value;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QString getInfo() override;

private:
    Value value;
    Value value_for_compute;
    friend SeqIn;
};

#endif // COMBIN_H
