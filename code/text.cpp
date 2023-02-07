#include "text.h"

Text::Text(Label *label): QGraphicsTextItem((QGraphicsItem *)label)
{
    belong = label;
    focus = true;
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void   Text::setBelong(Label *b)    {belong = b;    setParentItem((QGraphicsItem*)b);}
bool   Text::getFocus()             {return focus;}
Label* Text::getBelong()            {return belong;}

void Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter);
    QStyleOptionGraphicsItem op;
    if (option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;
    QGraphicsTextItem::paint(painter, &op, widget);
}

void Text::focusOutEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusOutEvent(event);
    focus = false;
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this->belong);
}

void Text::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseDoubleClickEvent(event);
    focus = true;
    setFocus();
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
}

void Text::keyPressEvent(QKeyEvent* event)
{
    if (event->key() != Qt::Key_Return)
    {
        // let parent implementation handle the event
        QGraphicsTextItem::keyPressEvent(event);
    }
    else
    {
        // ignore the event and stop its propagation
        event->accept();
        clearFocus();
        setSelected(false);
    }
}
