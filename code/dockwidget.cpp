#include "dockwidget.h"

DockWidget::DockWidget(QWidget *parent): QDockWidget(parent)
{
    setWindowTitle("simulation");

    scrollArea = new QScrollArea(this);
    widget = new QWidget;
    layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    if(mouse.isSeq())
    {
        l1 = new QHBoxLayout;
        start = new QSpinBox;
        end = new QSpinBox;
        QLabel *label1 = new QLabel;
        QLabel *label2 = new QLabel;

        label1->setText("time: ");
        label2->setText(" ~ ");
        start->setMinimumSize(QSize(45, 22));
        start->setRange(0, 99);
        start->setValue(0);
        start->setKeyboardTracking(false);
        end->setMinimumSize(QSize(45, 22));
        end->setRange(0, 99);
        end->setValue(40);
        end->setKeyboardTracking(false);

        connect(start, &QSpinBox::valueChanged, this, &DockWidget::startChange);
        connect(end, &QSpinBox::valueChanged, this, &DockWidget::endChange);

        l1->setContentsMargins(10, 10, 10, 5);
        l1->addStretch();
        l1->addWidget(label1);
        l1->addWidget(start);
        l1->addWidget(label2);
        l1->addWidget(end);
        layout->addLayout(l1);
    }
    setWidget(scrollArea);
}

DockWidget::~DockWidget()
{
    if(mouse.isSeq())
    {
        delete start;
        delete end;
        delete l1;
    }
    delete layout;
    delete widget;
    delete scrollArea;
}

void DockWidget::addWidget(Simul *simul)
{
    if(ids.contains(simul->getID()))  return;
    if(mouse.isSeq())
        dynamic_cast<Waveform *>(simul)->draw(start->value(), end->value());
    else
        dynamic_cast<Table *>(simul)->draw();
    ids.push_back(simul->getID());
    layout->addWidget(simul);
    connect(simul, &Simul::closed, this, &DockWidget::deleteSimul);
    reshape();
}

void DockWidget::closeEvent(QCloseEvent *event)
{
    emit closed();
    QDockWidget::closeEvent(event);
}

void DockWidget::deleteSimul(Simul *simul)
{
    ids.removeOne(simul->getID());
    delete simul;
    reshape();
}

void DockWidget::reshape()
{
    widget->deleteLater();
    widget = new QWidget;
    widget->setLayout(layout);

    scrollArea->deleteLater();
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(widget);
    scrollArea->setMinimumHeight(100);
    scrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setWidget(scrollArea);
}

void DockWidget::startChange(int i)
{
    if(i > end->value())
        start->setValue(end->value());
    for (int i = 0; i < layout->count(); ++i)
    {
        Waveform *waveform = dynamic_cast<Waveform *>(layout->itemAt(i)->widget());
        if (waveform)
            waveform->draw(start->value(), end->value());
    }
}

void DockWidget::endChange(int i)
{
    if(i < start->value())
        end->setValue(start->value());
    for (int i = 0; i < layout->count(); ++i)
    {
        Waveform *waveform = dynamic_cast<Waveform *>(layout->itemAt(i)->widget());
        if (waveform)
            waveform->draw(start->value(), end->value());
    }
}
