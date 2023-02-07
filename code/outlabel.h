#ifndef OUTLABEL_H
#define OUTLABEL_H

#include "label.h"

class OutLabel: public Label
{
public:
    OutLabel();
    OutLabel(QStringList);
    ~OutLabel();
    void setHover(){hover = true;}
    void setName(QString);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    QRectF boundingRect() const override;
    static int seq_num;
    static QVector<QString> names;

private:
    bool hover;
};

#endif // OUTLABEL_H
