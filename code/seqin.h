#ifndef SEQIN_H
#define SEQIN_H

#include "inlabel.h"
class CombIn;

class SeqIn: public InLabel
{
public:
    SeqIn();
    SeqIn(CombIn *);
    SeqIn(QStringList);
    void compute() override;
    void setValueType(QString vt)   {valuetype = vt;}
    void setValue(QString v)        {value = v;}
    void setDuplicate(bool b)       {duplicate = b;}
    void setValueForCompute(int);
    QString getValueType()          {return valuetype;}
    QString getValue()              {return value;}
    bool isDuplicate()              {return duplicate;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QString getInfo() override;

private:
    QString value;
    Value   value_for_compute;
    QString valuetype;
    bool duplicate;
    friend CombIn;
};

#endif // SEQIN_H
