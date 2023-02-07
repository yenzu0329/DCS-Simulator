#ifndef HISTORY_H
#define HISTORY_H

#include <QVector>
#include <QAction>
#include "circuit.h"
#include "outlabel.h"
#include "inlabel.h"

class History
{
public:
    History();
    void setActions(QAction *undo, QAction *redo);
    void addStep(Circuit *);
    void eraseHead();
    void delAfterCurrent();
    void clear();
    Circuit* undo(Circuit *);
    Circuit* redo();
private:
    const int maxlen;
    int current;
    QAction *undoAction;
    QAction *redoAction;
    QVector<Circuit *> steps;
    QVector<bool> comb_or_seq;
};

extern History history;
#endif // HISTORY_H
