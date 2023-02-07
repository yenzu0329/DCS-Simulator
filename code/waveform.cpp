#include "waveform.h"

Waveform::Waveform(Component *out, QWidget *parent) : Simul(out, parent)
{
    init = ZERO;
    trace(out, QList<Component *>());
    make_waveform(out);
}

void Waveform::make_waveform(Component * out)
{
    SeqIn *seqin;
    waveform = "";
    for(int i=0; i<100; i++)
    {
        for(InLabel *in: qAsConst(inputs))
        {
            seqin = (SeqIn *)in;
            seqin->setValueForCompute(i);
        }
        compute();
        switch (out->getInNodes().at(0)->getValue().type())
        {
        case ZERO:      waveform += '0';    break;
        case ONE:       waveform += '1';    break;
        case UNKNOWN:   waveform += 'x';    break;
        }
    }
}

void Waveform::draw(int start, int end)
{
    qreal h = 30;
    qreal w = 10 + 60 + 20 * 42;
    qreal interval = 20. * 42 / (end-start+1);
    QChar prev = '0';

    QPixmap *pixmap = new QPixmap(w, h);
    pixmap->fill(Qt::white);
    QPainter *painter = new QPainter(pixmap);
    painter->setPen(QPen(Qt::black, 0.5));
    for(int i=start; i<=end; i++)
    {
        if(waveform[i] == '0')
            painter->drawLine((i-start)*interval+5+60, h-6, (i-start+1)*interval+5+60, h-6);
        else if(waveform[i] == '1')
            painter->drawLine((i-start)*interval+5+60, 6, (i-start+1)*interval+5+60, 6);
        else
        {
            painter->setPen(QPen(Qt::red, 0.5));
            painter->drawLine((i-start)*interval+5+60, h-6, (i-start+1)*interval+5+60, h-6);
            painter->setPen(QPen(Qt::black, 0.5));
        }
        if(prev == '1' ^ waveform[i] == '1')
            painter->drawLine((i-start)*interval+5+60, 6, (i-start)*interval+5+60, h-6);
        prev = waveform[i];
    }
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->drawText(QRect(5, 8, 50, 20), Qt::AlignRight, tr(name.toStdString().c_str()));
    label->setPixmap(*pixmap);

    Simul::draw();
}
