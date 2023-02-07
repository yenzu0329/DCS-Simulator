#include "wire.h"

bool draw;
Wire::Wire(Node *node)
{
    type = "WIRE";
    connect(node);
    endPoints.push_back(node->scenePos());
    endPoints.push_back(node->scenePos());
    check.push_back(node);
    check.push_back(nullptr);

    hover = false;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

Wire::Wire(QStringList str, QStringList endpoints_str, QVector<void *> check)
{
    /*
     * 0: type
     * 1: id
     * 2: name
     * 3: pos_x
     * 4: pos_y
     */
    if(id > seq_num)    Component::seq_num = id;
    type = str[0];
    name = str[2];
    hover = false;
    setPos(QPointF(str[3].toFloat(), str[4].toFloat()));
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    int len = endpoints_str.length()-1;
    int i = 0;
    float x, y;
    while(i < len)
    {
        x = endpoints_str[i++].toFloat();
        y = endpoints_str[i++].toFloat();
        endPoints.push_back(QPointF(x, y));
    }
    for(auto k = check.begin(); k != check.end(); k++)
    {
        this->check.push_back(*k);
        if(*k != nullptr && *k != (void *)1)
            connect((Node *)*k);
    }
}

QPointF Wire::getPos(){return endPoints.last();}
void Wire::setHover(){hover = true;}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(hover || (mouse.getMode() != WIRE && isSelected() && !mouse.temp_select))
        painter->setPen(QPen(Qt::red, 1));
    else
        painter->setPen(QPen(Qt::black, 1));
    hover = false;
    draw  = false;
    for(int i=0; i<endPoints.length()-1; i++)
    {
        // draw line
        if(check[i])     draw = !draw;
        if(draw)
            painter->drawLine(endPoints[i], endPoints[i+1]);
    }
}

QRectF Wire::boundingRect() const
{
    QPointF topLeftPos(5000, 5000);
    QPointF bottomRightPos(0, 0);
    for (QPointF p: qAsConst(endPoints))
    {
        //set topLeftPos
        if(p.x() < topLeftPos.x())
            topLeftPos.setX(p.x());
        if(p.y() < topLeftPos.y())
            topLeftPos.setY(p.y());

        //set bottomRightPos
        if(p.x() > bottomRightPos.x())
            bottomRightPos.setX(p.x());
        if(p.y() > bottomRightPos.y())
            bottomRightPos.setY(p.y());
    }
    return QRectF(topLeftPos-QPointF(3,3), bottomRightPos+QPointF(3,3));
}

QPainterPath Wire::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    draw = false;
    for(int i=0; i<endPoints.length()-1; i++)
    {
        if(check[i])     draw = !draw;
        if(draw)
        {
            if(endPoints[i].x() < endPoints[i+1].x())
                path.addRect(QRectF(endPoints[i]+QPointF(3,-3), endPoints[i+1]+QPointF(-3,3)));
            else if(endPoints[i].y() < endPoints[i+1].y())
                path.addRect(QRectF(endPoints[i]+QPointF(-3,3), endPoints[i+1]+QPointF(3,-3)));
            else if(endPoints[i].x() > endPoints[i+1].x())
                path.addRect(QRectF(endPoints[i+1]+QPointF(3,-3), endPoints[i]+QPointF(-3,3)));
            else
                path.addRect(QRectF(endPoints[i+1]+QPointF(-3,3), endPoints[i]+QPointF(3,-3)));
        }
        path.addRect(QRectF(endPoints[i]-QPointF(3,3), endPoints[i]+QPointF(3,3)));
    }
    return path;
}

void Wire::reshape()
{
    Node *node;
    QPointF offset, temp;
    draw = false;
    fixPos();
    qDebug() << endPoints;
    qDebug() << check;
    for(int i=0; i<endPoints.length(); i++)
    {
        if(check[i])
        {
            draw = !draw;
            if(check[i] != (void *)1)
            {
                temp = endPoints[i];
                node = (Node *)check[i];
                endPoints[i] = node->scenePos();
                for(int j=0; j<endPoints.length(); j++)
                {
                    if(check[j] == (void *)1)
                    {
                        if(endPoints[j].y() == temp.y())
                            endPoints[j].setY(endPoints[i].y());
                    }
                }
                if(draw)
                {
                    offset = temp - endPoints[i+1];
                    if(offset.x() == 0) endPoints[i+1].setX(endPoints[i].x());
                    else                endPoints[i+1].setY(endPoints[i].y());
                }
                else
                {
                    offset = temp - endPoints[i-1];
                    if(offset.x() == 0) endPoints[i-1].setX(endPoints[i].x());
                    else                endPoints[i-1].setY(endPoints[i].y());
                }
            }
        }
    }
}

void Wire::fixPos()
{
    if(pos() != QPointF(0,0))
    {
        for(int i=0; i<endPoints.length(); i++)
            endPoints[i] += pos();
        setPos(QPointF(0,0));
        update();
    }
}

void Wire::changeLastEndPoint(QPointF p)
{
    QPointF offset = p - endPoints[endPoints.length()-2];
    if(abs(offset.x()) > abs(offset.y()))
    {
        endPoints.last().setX(p.x());
        endPoints.last().setY(endPoints[endPoints.length()-2].y());
    }
    else
    {
        endPoints.last().setX(endPoints[endPoints.length()-2].x());
        endPoints.last().setY(p.y());
    }
    update();
}

bool Wire::deleteLastEndPoint()
{
    do
    {
        endPoints.pop_back();
        check.pop_back();
    }
    while(check.last() == nullptr);

    endPoints.pop_back();
    check.pop_back();

    if(endPoints.empty())   return false;
    else                    return true;
}

void Wire::addEndPoint()
{
    int i;
    QPointF p = endPoints.last();
    i = endPoints.length()-3;
    if(i >= 0 && (endPoints[i].x() == p.x() || endPoints[i].y() == p.y()))
        endPoints[i+1] = p;
    else
    {
        endPoints.push_back(p);
        check.push_back(nullptr);
    }
}

bool Wire::connect(Node *node)
{
    if(dynamic_cast<InNode *>(node))
    {
        inNodes.push_back((InNode *)node);
        node->setConnect(this);
    }
    else
    {
        if(outNodes.empty())
        {
            outNodes.push_back((OutNode *)node);
            node->setConnect(this);
        }
        else //connect error
            return deleteLastEndPoint();
    }
    return true;
}

bool Wire::connectNode(Node *node)
{
    int i;
    QPointF offset, last;
    last = endPoints.last();

    // 避免橫線or直線上有重複的點
    i = endPoints.length()-3;
    if(i >= 0 && (endPoints[i].x() == last.x() || endPoints[i].y() == last.y()))
    {
        endPoints.pop_back();
        check.pop_back();
    }
    endPoints.last() = node->scenePos();
    check.last() = node;

    // 確保endpoints數量>=4
    while(endPoints.length() < 4)
    {
        endPoints.insert(1, endPoints.first());
        check.insert(1, nullptr);
    }

    // 調整最後兩個endpoints的位置
    i = endPoints.length()-2;
    offset = last - endPoints[i];
    if(!offset.x())
        endPoints[i].setX(endPoints.last().x());
    else if(!offset.y())
        endPoints[i].setY(endPoints.last().y());
    update();
    return connect(node);
}

bool Wire::connectWire(Wire *wire)
{
    //connect error
    wire->fixPos();
    if(!outNodes.empty() && !wire->outNodes.empty())
        return deleteLastEndPoint();

    // 避免橫線or直線上有重複的點
    int i;
    QPointF last;
    i = endPoints.length()-3;
    last = endPoints.last();
    if(i >= 0 && (endPoints[i].x() == last.x() || endPoints[i].y() == last.y()))
    {
        endPoints.pop_back();
        check.pop_back();
    }

    // 調整最後兩個endpoints的位置
    endPoints.last() = fixNode(last, wire);
    i = endPoints.length()-2;

    if(last.x() == endPoints[i].x())    endPoints[i].setX(endPoints.last().x());
    else                                endPoints[i].setY(endPoints.last().y());
    if(wire->check.last() != (void *)1)
        check.last() = (void *)1;
    if(endPoints.last().y() == endPoints[i].y())
    {
        endPoints.insert(endPoints.length()-1, endPoints.last()-QPointF(0.1, 0));
        check.insert(endPoints.length()-2, nullptr);
    }

    //copy "this" end points to "wire" end points
    for (i=endPoints.length()-1; i>=0; i--)
    {
        wire->endPoints.push_back(endPoints[i]);
        wire->check.push_back(check[i]);
    }

    //copy "this" node to "wire" node
    InNode *temp_in;
    OutNode *temp_out;
    while(!inNodes.empty())
    {
        temp_in = inNodes.first();
        inNodes.pop_front();
        wire->connect(temp_in);
    }
    if (!outNodes.empty())
    {
        temp_out = outNodes.first();
        outNodes.pop_front();
        wire->connect(temp_out);
    }
    update();
    return false;
}

QPointF Wire::fixNode(QPointF p, Wire *wire)
{
    QRectF box;
    QPointF pos, p1, p2;
    pos = p;
    auto min = [](double x1, double x2){return (x1 < x2) ? x1 : x2;};
    for(int i=1; i<wire->endPoints.length(); i++)
    {
        box.setRect(wire->endPoints[i].x()-5, wire->endPoints[i].y()-5, 10, 10);
        if(box.contains(p))
        {
            pos = wire->endPoints[i];
            break;
        }
        p1 = wire->endPoints[i];
        p2 = wire->endPoints[i-1];
        box.setRect(min(p1.x(), p2.x())-3, min(p1.y(), p2.y())-3, abs(p1.x()-p2.x())+6, abs(p1.y()-p2.y())+6);
        if(box.contains(p))
        {
            if(abs(p1.x()-p2.x()) == 0)
            {
                wire->endPoints.push_back(p1);
                wire->check.push_back((void *)1);
                pos.setX(p1.x());
                break;
            }
            else if(abs(p1.y()-p2.y()) == 0)    {pos.setY(p1.y());  break;}
        }
    }
    return pos;
}

void Wire::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    fixPos();
    QGraphicsItem::mousePressEvent(event);
    if(event->button() == Qt::RightButton)  return;
    if(mouse.getMode() == WIRE && !mouse.getWire())
    {
        QPointF pos = fixNode(event->scenePos(), this);
        mouse.setWire(this);
        endPoints.push_back(pos);
        endPoints.push_back(pos+QPointF(0, 0.1));
        endPoints.push_back(pos+QPointF(0, 0.1));
        if(check.last() != (void *)1)   check.push_back((void *)1);
        else                            check.push_back(nullptr);
        check.push_back(nullptr);
        check.push_back(nullptr);
    }
}

void Wire::compute()
{
    for(InNode* in: qAsConst(inNodes))
        in->setValue(outNodes[0]->getValue());
}

QString Wire::getInfo()
{
    QString info = Component::getInfo();
    info += '\n';
    for(QPointF p: qAsConst(endPoints))
        info += QString::number(p.x()) + ' ' + QString::number(p.y()) + ' ';
    info += '\n';
    for(void* c: qAsConst(check))
    {
        if(c == nullptr)        info += QString::number(0);
        else if(c == (void *)1) info += QString::number(1);
        else
        {
            Node *node = (Node *)c;
            info += QString::number(node->getID());
        }
        info += ' ';
    }
    return info;
}
