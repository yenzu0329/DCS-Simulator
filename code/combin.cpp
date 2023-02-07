#include "combin.h"
#include "seqin.h"

CombIn::CombIn()
{
    value = UNKNOWN;
}

CombIn::CombIn(QStringList str): InLabel(str)
{
    if(str.last() == "0")
    {
        text->setPlainText("0");
        value = ZERO;
    }
    else if(str.last() == "1")
    {
        text->setPlainText("1");
        value = ONE;
    }
    else
        value = UNKNOWN;
}

CombIn::CombIn(SeqIn *in): InLabel(in)
{
    if(in->getValueType() == "0")
        value = ZERO;
    else if(in->getValueType() == "1")
        value = ONE;
    else
    {
        text->setPlainText(getName());
        value = UNKNOWN;
    }
}

void CombIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(value.type() != UNKNOWN)
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

void CombIn::setValueForCompute(Value b)
{
    if(value.type() == UNKNOWN)
        value_for_compute = b;
    else
        value_for_compute = value;
    compute();
}

void CombIn::compute()
{
    outNodes[0]->setValue(value_for_compute);
    outNodes[0]->getConnect()->compute();
}

QString CombIn::getInfo()
{
    QString info = Component::getInfo();
    info += QString::number(value.type());
    return info;
}
