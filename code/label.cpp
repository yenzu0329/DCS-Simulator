#include "label.h"

Label::Label()
{
    type = "LABEL";
    text = new Text(this);
    text->setTextInteractionFlags(Qt::TextEditorInteraction);
    text->setFocus();
    text->setPos(-4, -12);
}

Label::Label(QStringList str): Component(str)
{
    text = new Text(this);
    text->setPlainText(name);
    text->setTextInteractionFlags(Qt::NoTextInteraction);
    text->setPos(-4, -12);
}

Label::Label(Label *label)
{
    // change text belong item
    text = label->text;
    text->setBelong(this);
    label->text = nullptr;
}

Label::~Label()
{
    if(text)    delete text;
}

void Label::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::lightGray, 0.5));
    if(text->getFocus() || isSelected())
        painter->drawRect(boundingRect());
}

QRectF Label::boundingRect() const
{
    qreal w = text->boundingRect().width();
    qreal h = text->boundingRect().height();
    QRectF r = QRectF(-4, -12, w, h);
    return r;
}

void Label::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(mouse.isSimul())    return;
    QGraphicsItem::mouseDoubleClickEvent(event);
    text->mouseDoubleClickEvent(event);
    text->mouseDoubleClickEvent(event);
}
