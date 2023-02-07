#include "history.h"

History history;
bool addnew = true;

History::History():maxlen(30)
{
    current = -1;
}

void History::addStep(Circuit *cir)
{
    QList<QGraphicsItem *> selects = cir->selectedItems();
    if(!addnew)
        delAfterCurrent();

    addnew = true;
    undoAction->setEnabled(true);
    redoAction->setEnabled(false);
    current++;
    cir->writeToFile("history.txt");
    Mouse copy_mouse = mouse;
    Circuit *copy_cir = new Circuit(0, 0, 5000, 5000);
    copy_cir->readFromFile("history.txt");
    mouse = copy_mouse;
    cir->resetNames();
    steps.push_back(copy_cir);
    comb_or_seq.push_back(mouse.isSeq());
    qDebug()  << "add" << steps[current];

    if(steps.length() > maxlen)
    {
        eraseHead();
        current--;
    }
    for (QGraphicsItem *item : qAsConst(selects))
        item->setSelected(true);
}

void History::eraseHead()
{
    Circuit *head_cir = steps.first();
    steps.pop_front();
    comb_or_seq.pop_front();
    head_cir->deleteLater();
}

void History::delAfterCurrent()
{
    while(current < steps.length()-1)
    {
        Circuit *last_cir = steps.last();
        steps.pop_back();
        comb_or_seq.pop_front();
        last_cir->deleteLater();
    }
    steps.pop_back();
    comb_or_seq.pop_front();
    current--;
}

void History::setActions(QAction *undo, QAction *redo)
{
    redoAction = redo;
    undoAction = undo;
}

void History::clear()
{
    while(!steps.empty())
    {
        Circuit *last_cir = steps.last();
        steps.pop_back();
        comb_or_seq.pop_front();
        last_cir->deleteLater();
    }
    current = -1;
    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    addnew = true;
}

Circuit* History::undo(Circuit *cir)
{
    if(addnew)
    {
        addStep(cir);
        addnew = false;
    }
    current--;
    redoAction->setEnabled(true);
    if(current == 0)
        undoAction->setEnabled(false);
    qDebug()  << "undo" << steps[current];
    mouse.setSeq(comb_or_seq[current]);
    return steps[current];
}

Circuit* History::redo()
{
    current++;
    undoAction->setEnabled(true);
    if(current == steps.length()-1)
        redoAction->setEnabled(false);
    qDebug()  << "redo" << steps[current];
    mouse.setSeq(comb_or_seq[current]);
    return steps[current];
}
