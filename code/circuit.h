#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QCursor>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include "and.h"
#include "or.h"
#include "nand.h"
#include "nor.h"
#include "xor.h"
#include "not.h"
#include "dff.h"
#include "tff.h"
#include "jkff.h"
#include "rsff.h"
#include "wire.h"
#include "label.h"
#include "outlabel.h"
#include "combin.h"
#include "seqin.h"
#include "mouse.h"
#include "selectbox.h"
#include "gateinfo.h"
#include "ffinfo.h"
#include "combininfo.h"
#include "seqininfo.h"
#include "table.h"
#include "waveform.h"

class Circuit: public QGraphicsScene
{
public:
    Circuit(qreal x, qreal y, qreal width, qreal height);
    ~Circuit();
    void init();  // for undo and redo
    void setMovable(bool);
    void setCenterPos(QPointF p){centerPos = p;}
    void closeSelectBox();
    bool checkError();
    void findComponents(QString);
    void writeToFile(QString);
    void readFromFile(QString);
    void centerOn();
    void resetNames();
    Node* findNodeByID(int);
    SelectBox* getSelectBox(){return selectBox;}
    QPointF getCenterPos(){return centerPos;}

private:
    QGraphicsPixmapItem *bgItem;
    QGraphicsLineItem *hLine, *vLine;
    QPointF centerPos;
    Wire *curr_wire;
    SelectBox *selectBox;

private slots:
    void changeMode();
    void textLostFocus(Label *item);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#include "history.h"
#endif // CIRCUIT_H
