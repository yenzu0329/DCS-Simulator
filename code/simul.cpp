#include "simul.h"

Simul::Simul(Component *c, QWidget *parent) : QWidget(parent)
{
    id = c->getID();
    name = c->getName();
    close_btn = new QToolButton;
    label = new QLabel;
    layout = new QHBoxLayout;
}

Simul::~Simul()
{
    delete label;
    delete layout;
    delete close_btn;
}

void Simul::trace(Component *c, QList<Component *> path)
{
    if(dynamic_cast<InLabel *>(c))
    {
        InLabel *in = (InLabel *)c;
        inputs.insert(in);
        return;
    }
    if(path.contains(c))        return;
    QVector<InNode *> inNodes = c->getInNodes();
    for(InNode *in: qAsConst(inNodes))
    {
        in->setValue(init);
        in->getConnect()->getOutNodes().first()->setValue(init);
        path.push_back(c);
        trace(in->getConnect()->getOutNodes().first()->getBelong(), path);
    }
    if(dynamic_cast<FlipFlop *>(c))
    {
        InNode *clk = dynamic_cast<FlipFlop *>(c)->getClock();
        dynamic_cast<FlipFlop *>(c)->init();
        clk->getConnect()->getOutNodes().first()->setValue(init);
        trace(clk->getConnect()->getOutNodes().first()->getBelong(), path);
    }
    return;
}

void Simul::compute()
{
    Component *c;
    QVector<Component *> next;
    QVector<Component *> record;
    QVector<InNode *>inNodes;
    QVector<OutNode *>outNodes;
    for(InLabel *in: qAsConst(inputs))
    {
        record.push_back(in);
        inNodes = in->getOutNodes().at(0)->getConnect()->getInNodes();
        for(InNode* n: qAsConst(inNodes))
            next.push_back(n->getBelong());
    }
    while(!next.empty())
    {
        c = next.first();
        next.pop_front();
        c->compute();
        if(record.contains(c))  continue;
        record.push_back(c);
        outNodes = c->getOutNodes();
        for(OutNode* out: qAsConst(outNodes))
            if(out->getConnect())
            {
                inNodes = out->getConnect()->getInNodes();
                for(auto n = inNodes.begin(); n != inNodes.end(); n++)
                    next.push_back((*n)->getBelong());
            }
    }
}

void Simul::draw()
{
    // draw close button
    close_btn->setIcon(QIcon(":/icons/close.png"));
    connect(close_btn, &QToolButton::clicked, this, &Simul::close_btn_clicked);

    // put button & label on the layout
    layout->addWidget(close_btn);
    layout->addWidget(label);
    layout->setContentsMargins(10, 0, 10, 0);
    setLayout(layout);
}

void Simul::close_btn_clicked()
{
    qDebug() << "close simul";
    emit closed(this);
}
