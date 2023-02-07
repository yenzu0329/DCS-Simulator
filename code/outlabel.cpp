#include "outlabel.h"

int OutLabel::seq_num = 0;
QVector<QString> OutLabel::names;

OutLabel::OutLabel()
{
    type = "OUT";
    do
    {name = "out_"+QString::number(seq_num++);}
    while(names.contains(name));
    names.push_back(name);
    text->setPlainText(name);
    text->setTextInteractionFlags(Qt::NoTextInteraction);
    text->clearFocus();
    InNode *in = new InNode(this);
    in->setScale(0.1);
    inNodes.push_back(in);
    inNodes[0]->setPos(-7, 0);
    text->setPos(-6, -12);
}

OutLabel::OutLabel(QStringList str): Label(str)
{
    names.push_back(name);
    inNodes[0]->setScale(0.1);
    inNodes[0]->setPos(-7, 0);
    text->setPos(-6, -12);
}

OutLabel::~OutLabel()
{
    if(names.contains(name))
        names.removeOne(name);
}

void OutLabel::setName(QString s)
{
    if(s == "") return;
    if(!names.contains(s))
    {
        if(names.contains(name))
            names[names.indexOf(name)] = s;
        else
            names.push_back(s);
        name = s;
    }
    else
        text->setPlainText(name);
}

void OutLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath path;
    qreal w = text->boundingRect().width();
    qreal h = text->boundingRect().height();
    path.moveTo(-7, -h/2+1);
    path.lineTo(w-7, -h/2+1);
    path.lineTo(w, 0);
    path.lineTo(w-7, +h/2-1);
    path.lineTo(-7, +h/2-1);
    path.lineTo(-7, -h/2+1);

    if(isSelected() && !mouse.temp_select)
        painter->setPen(QPen(Qt::red, 3));
    else
        painter->setPen(QPen(Qt::black, 3));
    painter->drawPath(path);
}

QRectF OutLabel::boundingRect() const
{
    qreal w = text->boundingRect().width();
    qreal h = text->boundingRect().height();
    QRectF r = QRectF(-8, -h/2, w+8, h);
    return r;
}
