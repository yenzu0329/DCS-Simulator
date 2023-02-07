#include "selectbox.h"

SelectBox::SelectBox()
{
    box = new QRectF(0, 0, 0, 0);
    pen = new QPen(Qt::gray, 0.5, Qt::DashLine);

    setRect(*box);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    acceptedMouseButtons();
}

SelectBox::SelectBox(QPointF pos)
{
    box = new QRectF(pos, pos);
    pen = new QPen(Qt::gray, 0.5, Qt::DashLine);
    startPos = pos;

    setRect(*box);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    acceptedMouseButtons();
}

SelectBox::~SelectBox()
{
    delete box;
    delete pen;
}

void SelectBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(*pen);
    painter->drawRect(*box);
}


QRectF SelectBox::boundingRect() const
{
    return QRectF(*box);
}

QPainterPath SelectBox::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void SelectBox::setStartPos(QPointF pos)
{
    startPos = pos;
    box->setRect(pos.x(), pos.y(), 0, 0);
    setRect(*box);
}

void SelectBox::setEndPos(QPointF endPos)
{
    QPointF topLeftPos, bottomRightPos;
    if(startPos.x() < endPos.x())
    {
        topLeftPos.setX(startPos.x());
        bottomRightPos.setX(endPos.x());
    }
    else
    {
        topLeftPos.setX(endPos.x());
        bottomRightPos.setX(startPos.x());
    }

    if(startPos.y() < endPos.y())
    {
        topLeftPos.setY(startPos.y());
        bottomRightPos.setY(endPos.y());
    }
    else
    {
        topLeftPos.setY(endPos.y());
        bottomRightPos.setY(startPos.y());
    }
    box->setTopLeft(topLeftPos);
    box->setBottomRight(bottomRightPos);
    setRect(*box);
}

void SelectBox::reshape()
{
    QList<QGraphicsItem *> items = scene()->selectedItems();
    QPointF topLeftPos(5000, 5000);
    QPointF bottomRightPos(0, 0);
    for (QGraphicsItem *item : qAsConst(items))
    {
        if(item != this)
        {
            //set topLeftPos
            if(item->sceneBoundingRect().topLeft().x() < topLeftPos.x())
                topLeftPos.setX(item->sceneBoundingRect().topLeft().x());
            if(item->sceneBoundingRect().topLeft().y() < topLeftPos.y())
                topLeftPos.setY(item->sceneBoundingRect().topLeft().y());

            //set bottomRightPos
            if(item->sceneBoundingRect().bottomRight().x() > bottomRightPos.x())
                bottomRightPos.setX(item->sceneBoundingRect().bottomRight().x());
            if(item->sceneBoundingRect().bottomRight().y() > bottomRightPos.y())
                bottomRightPos.setY(item->sceneBoundingRect().bottomRight().y());
        }
    }
    box->setTopLeft(topLeftPos);
    box->setBottomRight(bottomRightPos);
    setRect(*box);
}

void SelectBox::fixPos()
{
    if(pos() != QPointF(0, 0))
    {
        box->moveTopLeft(box->topLeft() + pos());
        setPos(QPointF(0, 0));
        setRect(*box);
    }
}
