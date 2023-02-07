#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QKeyEvent>

class Label;

class Text: public QGraphicsTextItem
{
    Q_OBJECT
public:
    Text(Label *);
    void setBelong(Label *);
    Label *getBelong();
    bool getFocus();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void lostFocus(Label *item);

private:
    Label *belong;
    bool focus;
};

#endif // TEXT_H
