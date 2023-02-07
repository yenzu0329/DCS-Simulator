#include "inlabel.h"

int InLabel::seq_num = 0;
QVector<QString> InLabel::names;

InLabel::InLabel()
{
    type = "IN";
    do
    {name = "in_"+QString::number(seq_num++);}
    while(names.contains(name));
    names.push_back(name);
    text->setPlainText(name);
    text->setTextInteractionFlags(Qt::NoTextInteraction);
    text->clearFocus();
    OutNode *out = new OutNode(this);
    out->setScale(0.1);
    outNodes.push_back(out);
    outNodes[0]->setPos(7, 0);
}

InLabel::InLabel(QStringList str): Label(str)
{
    names.push_back(name);
    outNodes[0]->setScale(0.1);
    outNodes[0]->setPos(7, 0);
}

InLabel::InLabel(InLabel* label):Label(label)
{
    type = "IN";
    name = label->name;
    names.push_back(name);
    item_scene = label->item_scene;
    if(mouse.getMode() == NONE)
        setFlag(QGraphicsItem::ItemIsMovable, true);

    // change outnode belong item
    label->outNodes[0]->setBelong(this);
    outNodes.push_back(label->outNodes[0]);
    label->outNodes.pop_back();

    setPos(label->scenePos());
}

InLabel::~InLabel()
{
    if(names.contains(name))
        names.removeOne(name);
}

void InLabel::setName(QString s)
{
    if(s == "") return;
    if(!names.contains(s))
    {
        names[names.indexOf(name)] = s;
        name = s;
    }
}

void InLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath path;
    qreal w = text->boundingRect().width();
    qreal h = text->boundingRect().height();

    if(isSelected() && !mouse.temp_select)
        painter->setPen(QPen(Qt::red, 3));
    else
        painter->setPen(QPen(Qt::black, 3));

    path.moveTo(-w, -h/2+1);
    path.lineTo(0, -h/2+1);
    path.lineTo(7, 0);
    path.lineTo(0, +h/2-1);
    path.lineTo(-w, +h/2-1);
    path.lineTo(-w, -h/2+1);
    painter->drawPath(path);
    text->setPos(1-w, -12);
}

QRectF InLabel::boundingRect() const
{
    qreal w = text->boundingRect().width();
    qreal h = text->boundingRect().height();
    QRectF r = QRectF(-1-w, -h/2, w+8, h);
    return r;
}

void InLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{}
