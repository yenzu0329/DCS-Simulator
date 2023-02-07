#ifndef SIMUL_H
#define SIMUL_H

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QLabel>
#include "inlabel.h"
#include "outlabel.h"

class Simul : public QWidget
{
    Q_OBJECT
public:
    explicit Simul(Component *,QWidget *parent = nullptr);
    ~Simul();
    int getID(){return id;}
    void trace(Component *, QList<Component *> path);
    void compute();
    void close_btn_clicked();
    void draw();

signals:
    void closed(Simul *);

protected:
    int id;
    QString name;
    QSet<InLabel *> inputs;
    QToolButton *close_btn;
    QLabel *label;
    QHBoxLayout *layout;
    Value init;
};

#endif // SIMUL_H
