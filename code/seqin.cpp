#include "seqin.h"
#include "combin.h"

SeqIn::SeqIn()
{
    duplicate = true;
    value = "01";
    valuetype = "user-defined";
}  

SeqIn::SeqIn(QStringList str): InLabel(str)
{
    /*
     * -3: valuetype (0, 1, clock, user-defined)
     * -2: value (0, 1, ...)
     * -1: duplicate (0 or 1)
     */
    int len = str.length();
    duplicate = str[len-1].toInt();
    value = str[len-2];
    valuetype = str[len-3];
    if(valuetype != "user-defined")
        text->setPlainText(valuetype);
}

SeqIn::SeqIn(CombIn *in): InLabel(in)
{
    duplicate = true;
    if(in->getValue().type() == ONE)
        value = valuetype = "1";
    else if(in->getValue().type() == ZERO)
        value = valuetype = "0";
    else
    {
        value = "01";
        valuetype = "user-defined";
    }
}

void SeqIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QString t = getValueType();
    if(t != "user-defined")
    {
        text->setDefaultTextColor(Qt::white);
        if(isSelected() && !mouse.temp_select)
            painter->setBrush(Qt::red);
        else
            painter->setBrush(Qt::black);
    }
    else
    {
        text->setDefaultTextColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
    }
    InLabel::paint(painter, option, widget);
}

void SeqIn::setValueForCompute(int time)
{
    int len = value.length();
    if(duplicate)
        value_for_compute = value[time % len].toLatin1() - '0';
    else if(time < len)
        value_for_compute = value[time].toLatin1() - '0';
    else
        value_for_compute = value[len-1].toLatin1() - '0';
    compute();
}

void SeqIn::compute()
{
    outNodes[0]->setValue(value_for_compute);
    outNodes[0]->getConnect()->compute();
}

QString SeqIn::getInfo()
{
    QString info = Component::getInfo();
    info += valuetype + '$' + value + '$' + QString::number(duplicate);
    return info;
}
