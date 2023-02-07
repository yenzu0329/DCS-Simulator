#include "table.h"

Table::Table(Component *out, QWidget *parent) : Simul(out, parent)
{
    init = UNKNOWN;
    trace(out, QList<Component *>());
    make_table(out);
}

Table::~Table()
{
    for(int i=0; i<row_num; i++)
        delete table[i];
    delete table;
}

void Table::make_table(Component *out)
{
    CombIn *combin;
    for(InLabel *in: qAsConst(inputs))
    {
        combin = dynamic_cast<CombIn *>(in);
        if(combin->getValue().type() == UNKNOWN)
            variables.insert(combin);
    }
    row_num = pow(2, variables.size());
    column_num = variables.size()+1;

    table = new Value* [row_num];
    for(int i=0; i<row_num; i++)
        table[i] = d2b(i);

    for(int i=0; i<row_num; i++)
    {
        int j=0;
        trace(out, QList<Component *>());
        for(auto k = inputs.begin(); k != inputs.end(); k++)
        {
            CombIn *in = dynamic_cast<CombIn *>(*k);
            in->setValueForCompute(UNKNOWN);
        }
        for(auto k = variables.begin(); k != variables.end(); k++)
            (*k)->setValueForCompute(table[i][j++]);
        compute();
        table[i][j] = out->getInNodes().at(0)->getValue().type();
    }
}

Value* Table::d2b(int dec)
{
    Value *b = new Value[column_num];
    int flag = 1;
    for(int i = column_num-2; i>=0; i--)
    {
        b[i] = bool(dec & flag);
        flag <<= 1;
    }
    return b;
}

void Table::draw()
{
    int i, j;
    qreal h = 20 + (row_num+1) * 24;
    qreal w = 20 + column_num * 60;

    QPixmap *pixmap = new QPixmap(w, h);
    pixmap->fill(Qt::white);
    QPainter *painter = new QPainter(pixmap);

    // draw lines
    painter->setPen(QPen(Qt::gray, 1));
    for(i=0; i <= row_num+1; i++)
        painter->drawLine(10, 10+i*24, w-10, 10+i*24);
    for(j=0; j <= column_num; j++)
        painter->drawLine(10+j*60, 10, 10+j*60, h-10);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(w-70, 12, w-70, h-10);
    painter->drawLine(12, 34, w-10, 34);

    // draw nodes name
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(QPen(Qt::black, 1));
    j=0;
    for(auto k = variables.begin(); k != variables.end(); k++)
    {
        painter->drawText(QRect(10+j*60, 10, 60, 24), Qt::AlignCenter, tr((*k)->getName().toStdString().c_str()));
        j++;
    }
    painter->drawText(QRect(10+j*60, 10, 60, 24), Qt::AlignCenter, tr(name.toStdString().c_str()));
    // draw values
    for(i=0; i < row_num; i++)
        for(j=0; j < column_num; j++)
        {
            if(table[i][j] == ONE)
                painter->drawText(QRect(10+j*60, 34+i*24, 60, 24), Qt::AlignCenter, tr("1"));
            else if(table[i][j] == ZERO)
                painter->drawText(QRect(10+j*60, 34+i*24, 60, 24), Qt::AlignCenter, tr("0"));
            else
            {
                painter->setPen(QPen(Qt::red, 1));
                painter->drawText(QRect(10+j*60, 34+i*24, 60, 24), Qt::AlignCenter, tr("X"));
                painter->setPen(QPen(Qt::black, 1));
            }
        }
    label->setPixmap(*pixmap);

    Simul::draw();
}
