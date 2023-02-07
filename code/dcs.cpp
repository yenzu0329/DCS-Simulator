#include "dcs.h"

DCS::DCS(QWidget *parent): QGraphicsView(parent)
{
    circuit = new Circuit(0, 0, 5000, 5000);
    this->setScene(circuit);
    setMouseTracking(true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void DCS::setNewCircuit()
{
    circuit->deleteLater();
    circuit = new Circuit(0, 0, 5000, 5000);
    this->setScene(circuit);

    // init static data members
    Component::seq_num = 0;
    OutLabel::seq_num = 0;
    InLabel::seq_num = 0;
    Node::seq_num = 2;
}

int DCS::checkFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))     return 1;
    QTextStream in(&file);
    QStringList words = in.readLine().split(" ");
    if(words[0] != "CIRCUIT")   return 2;
    return 0;
}

void DCS::setCombCircuit()
{
    if(!mouse.isSeq()) return;
    history.addStep(circuit);
    mouse.setSeq(false);

    QList<QGraphicsItem *> items = circuit->items();
    for (QGraphicsItem *item : qAsConst(items))
    {
        if(dynamic_cast<FlipFlop *>(item))
        {
            circuit->removeItem(item);
            delete item;
        }
        else if(dynamic_cast<SeqIn *>(item))
        {
            CombIn *in = new CombIn(dynamic_cast<SeqIn *>(item));
            circuit->addItem(in);
            circuit->removeItem(item);
            delete item;
        }
    }
}

void DCS::setSeqCircuit()
{
    if(mouse.isSeq()) return;
    history.addStep(circuit);
    mouse.setSeq(true);

    QList<QGraphicsItem *> items = circuit->items();
    for (QGraphicsItem *item : qAsConst(items))
    {
        if(dynamic_cast<CombIn *>(item))
        {
            SeqIn *in = new SeqIn(dynamic_cast<CombIn *>(item));
            circuit->addItem(in);
            circuit->removeItem(item);
            delete item;
        }
    }
}

void DCS::setCircuit(Circuit* cir)
{
    circuit = cir;
    setScene(circuit);
    update();
}

bool DCS::startSimul()
{
    mouse.setSimul(true);
    bool check = circuit->checkError();
    if(!check)
        mouse.setSimul(false);
    return check;
}

void DCS::stopSimul()
{
    mouse.setSimul(false);
}

void DCS::wheelEvent(QWheelEvent *event)
{
    int wheelValue = event->angleDelta().y();
    double radio = wheelValue / 1200. + 1;
    if(mouse.getRadio()*radio < 5 && mouse.getRadio()*radio > 0.5)
    {
        mouse.setRadio(radio);
        mouse.changeBackgroundCursor();
        //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        scale(radio, radio);
    }
}

void DCS::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    centerOn(circuit->getCenterPos());
}
