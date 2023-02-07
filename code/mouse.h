#ifndef MOUSE_H
#define MOUSE_H

#include <QAction>
#include <QString>
#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

enum Mode {NONE, WIRE, SELECT, ADD};
enum Type {AND, OR, NAND, NOR, XOR, NOT, DFF, RSFF, TFF, JKFF, IN, OUT, LABEL};

class Node;
class Wire;
class Simul;

class Mouse
{
public:    
    Mouse();
    void setMode(Mode);
    void setType(Type);
    void setRadio(double);
    void setNode(Node*);
    void setWire(Wire*);
    void setBackground(QGraphicsItem *);
    void setSimul(bool);
    void setSeq(bool);
    bool isSimul();
    bool isSeq();
    int  getMode();
    int  getType();
    double getRadio();
    Node* getNode();
    Wire* getWire();
    void changeBackgroundCursor();
    int  stringToType(QString str);
    QString typeToString(Type t);

    QAction *changeAction;
    QAction *showSimul;
    Simul *simul;
    bool temp_select;

private:
    Mode mode;
    Type type;
    QGraphicsItem *background;
    Node *node;
    Wire *wire;
    double radio;
    bool is_simul;
    bool is_seq;
};

extern Mouse mouse;
#endif // MOUSE_H
