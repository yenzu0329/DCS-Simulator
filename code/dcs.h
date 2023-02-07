#ifndef DCS_H
#define DCS_H

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "circuit.h"
#include "mouse.h"

class DCS : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DCS(QWidget *parent);
    void resizeEvent(QResizeEvent *) override;
    void setCircuit(Circuit *);
    void setCombCircuit();
    void setSeqCircuit();
    bool startSimul();
    void stopSimul();
    void setNewCircuit();
    int  checkFile(QString);
    Circuit* getCircuit(){return circuit;}

private:
    Circuit* circuit;

private slots:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // DCS_H
