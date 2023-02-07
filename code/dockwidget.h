#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include <QScrollArea>
#include <QBoxLayout>
#include <QObject>
#include <QDebug>
#include <QSpinBox>
#include "table.h"
#include "waveform.h"
#include "mouse.h"

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    DockWidget(QWidget *parent = nullptr);
    ~DockWidget();
    void addWidget(Simul *);
    void closeEvent(QCloseEvent *event) override;
    void deleteSimul(Simul *);
    void reshape();
    void startChange(int i);
    void endChange(int i);
signals:
    void closed();

private:
    QScrollArea *scrollArea;
    QWidget *widget;
    QVBoxLayout *layout;
    QHBoxLayout *l1;
    QVector<int> ids;
    QSpinBox *start, *end; // for seq
};

#endif // DOCKWIDGET_H
