#include "mainwindow.h"
#include "ui_mainwindow.h"

int cNum = 13;  // number of component buttons

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // connect signals & slots
    mouse.changeAction = new QAction(tr("change mode"));
    mouse.showSimul = new QAction(tr("show simul"));
    connect(mouse.changeAction, &QAction::triggered, this, &MainWindow::changeMode);
    connect(mouse.showSimul, &QAction::triggered, this, &MainWindow::showSimul);

    // setup UI
    filename = "";
    ui->setupUi(this);
    setButton();
    setToolBars();

    // init history
    history.setActions(ui->actionUndo, ui->actionRedo);
    history.clear();

    // 排版
    QWidget *widgetLeft = new QWidget;
    widgetLeft->setLayout(ui->layoutLeft);

    QWidget *widgetRight = new QWidget;
    widgetRight->setLayout(ui->layoutRight);

    QHBoxLayout *layoutMain = new QHBoxLayout;
    layoutMain->addWidget(widgetLeft);
    layoutMain->addWidget(widgetRight);

    QWidget *widget = new QWidget;
    widget->setLayout(layoutMain);
    setCentralWidget(widget);
    ui->toolBox->setCurrentWidget(ui->pageGate);

    // 設定lineEdit提示字
    ui->lineEditFile->setPlaceholderText("File Name");
    ui->lineEditSearch->setPlaceholderText("Search");
}

MainWindow::~MainWindow()
{
    delete buttonGroup;
    delete ui;
}

void MainWindow::setButton()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup,  QOverload<int>::of(&QButtonGroup::idClicked),this, &MainWindow::buttonGroupClicked);
    buttonGroup->addButton(ui->toolButtonAnd, AND);
    buttonGroup->addButton(ui->toolButtonOr, OR);
    buttonGroup->addButton(ui->toolButtonNand, NAND);
    buttonGroup->addButton(ui->toolButtonNor, NOR);
    buttonGroup->addButton(ui->toolButtonNot, NOT);
    buttonGroup->addButton(ui->toolButtonXor, XOR);
    buttonGroup->addButton(ui->toolButtonDff, DFF);
    buttonGroup->addButton(ui->toolButtonTff, TFF);
    buttonGroup->addButton(ui->toolButtonRSff, RSFF);
    buttonGroup->addButton(ui->toolButtonJKff, JKFF);
    buttonGroup->addButton(ui->toolButtonInput, IN);
    buttonGroup->addButton(ui->toolButtonOutput, OUT);
    buttonGroup->addButton(ui->toolButtonText, LABEL);
}

void MainWindow::setToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(ui->actionNew);
    fileToolBar->addAction(ui->actionOpen);
    fileToolBar->addAction(ui->actionSave);
    fileToolBar->setIconSize(QSize(23,23));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(ui->actionUndo);
    editToolBar->addAction(ui->actionRedo);
    editToolBar->addAction(ui->actionSelect);
    editToolBar->addAction(ui->actionWire);
    editToolBar->addAction(ui->actionDelete);
    editToolBar->addAction(ui->actionCenter_On);
    editToolBar->setIconSize(QSize(23,23));

    simulToolBar = addToolBar(tr("Simulate"));
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    simulToolBar->addWidget(spacer);
    simulToolBar->addWidget(ui->pushButtonSimul);
    ui->pushButtonSimul->addAction(ui->actionRun);
}

void MainWindow::buttonGroupClicked(int id)
{
    //改變滑鼠狀態
    for(int i=0; i<cNum; i++)
        if(i != id)
            buttonGroup->button(i)->setChecked(false);
    if(buttonGroup->button(id)->isChecked())
    {
        mouse.setType((Type)id);
        mouse.setMode(ADD);
    }
    else
        mouse.setMode(NONE);

    //取消select, wire按鈕
    ui->actionSelect->setChecked(false);
    ui->actionWire->setChecked(false);
}

void MainWindow::on_radioButtonComb_clicked()
{
    ui->pageFF->setEnabled(false);
    ui->dcs->setCombCircuit();
}

void MainWindow::on_radioButtonSeq_clicked()
{
    ui->pageFF->setEnabled(true);
    ui->dcs->setSeqCircuit();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //按右鍵 -> 取消所有按鈕
    if(event->button() == Qt::RightButton)
    {
        if(mouse.getWire())
        {
            mouse.setNode(nullptr);
            mouse.setWire(nullptr);
        }
        else
            mouse.setMode(NONE);
    }
}

void MainWindow::changeMode()
{
    if(mouse.getMode() == NONE)
    {
        ui->actionSelect->setChecked(false);
        ui->actionWire->setChecked(false);
        buttonGroup->button(mouse.getType())->setChecked(false);
    }
}


void MainWindow::on_actionDelete_triggered()
{
    Circuit *circuit = ui->dcs->getCircuit();
    circuit->getSelectBox()->setSelected(false);
    circuit->getSelectBox()->setVisible (false);
    QList<QGraphicsItem *> selectedItems = circuit->selectedItems();
    if(!selectedItems.empty())
    {
        history.addStep(circuit);
        for (QGraphicsItem *item : qAsConst(selectedItems))
        {
            if(item->scene() == circuit)
            {
                circuit->removeItem(item);
                delete item;
            }
        }
    }
}


void MainWindow::on_actionWire_triggered()
{
    if(ui->actionWire->isChecked())
    {
        mouse.setMode(WIRE);
        ui->actionSelect->setChecked(false);
        buttonGroup->button(mouse.getType())->setChecked(false);
    }
    else
        mouse.setMode(NONE);
}


void MainWindow::on_actionSelect_triggered()
{
    if(ui->actionSelect->isChecked())
    {
        mouse.setMode(SELECT);
        ui->actionWire->setChecked(false);
        buttonGroup->button(mouse.getType())->setChecked(false);
    }
    else
        mouse.setMode(NONE);
}


void MainWindow::on_pushButtonSimul_clicked()
{
    if(ui->pushButtonSimul->isChecked())
    {
        ui->pushButtonSimul->setIcon(QIcon(":/icons/stop.png"));
        ui->pushButtonSimul->setText("Stop");
        ui->pushButtonSimul->setStyleSheet("background-color: rgb(224, 79, 95);\
                                            color: rgb(255, 255, 255);\
                                            border-radius: 4px;\
                                            padding: 1px 4px;\
                                            border-style: outset;");
        ui->actionRun->trigger();
    }
    else
    {
        ui->pushButtonSimul->setIcon(QIcon(":/icons/run.png"));
        ui->pushButtonSimul->setText("Run");
        ui->pushButtonSimul->setStyleSheet("background-color: rgb(50, 190, 166);\
                                            color: rgb(255, 255, 255);\
                                            border-radius: 4px;\
                                            padding: 1px 4px;\
                                            border-style: outset;");
        ui->actionStop->trigger();
    }
}

void MainWindow::setActionEnable(bool b)
{
    ui->actionRun->setEnabled(b);
    ui->actionStop->setEnabled(!b);
    ui->actionDelete->setEnabled(b);
    ui->actionWire->setEnabled(b);
    ui->actionSelect->setEnabled(b);
    ui->groupBox->setEnabled(b);
    for(int i=0; i<cNum; i++)
        buttonGroup->button(i)->setEnabled(b);
}

void MainWindow::on_actionRun_triggered()
{
    mouse.setMode(NONE);
    if(!ui->dcs->startSimul())
    {
        if(ui->pushButtonSimul->isChecked())
            ui->pushButtonSimul->click();
        return;
    }
    setActionEnable(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    if(!ui->pushButtonSimul->isChecked())
        ui->pushButtonSimul->click();

    // create a dock widget
    dockwidget = new DockWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea, dockwidget);
    connect(dockwidget, &DockWidget::closed, this, &MainWindow::on_actionStop_triggered);
}

void MainWindow::on_actionStop_triggered()
{
    setActionEnable(true);
    ui->actionStop->setEnabled(false);
    ui->actionRun->setEnabled(true);
    if(ui->pushButtonSimul->isChecked())
        ui->pushButtonSimul->click();
    ui->dcs->stopSimul();
    dockwidget->deleteLater();
}

void MainWindow::on_lineEditSearch_returnPressed()
{
    on_toolButtonSearch_clicked();
}

void MainWindow::on_toolButtonSearch_clicked()
{
    QString keyword = ui->lineEditSearch->text().toLower();
    for(int i=AND; i<DFF; i++)
        if(buttonGroup->button(i)->text().contains(keyword))
            ui->toolBox->setCurrentWidget(ui->pageGate);
    for(int i=DFF; i<IN; i++)
        if(buttonGroup->button(i)->text().contains(keyword))
            ui->toolBox->setCurrentWidget(ui->pageFF);
    for(int i=IN; i<cNum; i++)
        if(buttonGroup->button(i)->text().contains(keyword))
            ui->toolBox->setCurrentWidget(ui->pageLabel);
    ui->dcs->getCircuit()->findComponents(keyword);
}

void MainWindow::showSimul()
{
    dockwidget->addWidget(mouse.simul);
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fileDialog;
    QString f = fileDialog.getOpenFileName(this, "Open File", ".", "Text files (*.txt)");
    int err;
    if(f != "")
    {
        qDebug() << "read file from " + f;
        err = ui->dcs->checkFile(f);
        if(!err)
        {
            if(mouse.isSimul()) on_actionStop_triggered();
            // create a new circuit and read file
            ui->dcs->setNewCircuit();
            ui->dcs->getCircuit()->readFromFile(f);
            ui->dcs->getCircuit()->centerOn();
            // change lineEdit
            filename = f;
            f = f.split("/").last();
            f = f.split(".").first();
            ui->lineEditFile->setText(f);
            if(mouse.isSeq())   ui->radioButtonSeq->click();
            else                ui->radioButtonComb->click();
            history.clear();
        }
        else if(err == 1)
            QMessageBox::critical(this, "Error", "File cannot open!", QMessageBox::Ok, QMessageBox::Ok);
        else if(err == 2)
            QMessageBox::critical(this, "Error", "File format error!", QMessageBox::Ok, QMessageBox::Ok);
    }
}

bool MainWindow::on_actionSave_triggered()
{
    bool b;
    QString f = ui->lineEditFile->text();
    if(filename == "")      b = true;
    else
    {
        QString filename = this->filename.split("/").last().split(".").first();
        if(f != filename)   b = true;
        else                b = false;
    }
    if(b)
    {
        QFileDialog fileDialog;
        f = fileDialog.getSaveFileName(this, "Save As", "./"+f, "Text files (*.txt)");
        if(f != "")
        {
            filename = f;
            f = f.split("/").last();
            f = f.split(".").first();
            ui->lineEditFile->setText(f);
        }
        else return false;
    }
    ui->dcs->getCircuit()->writeToFile(filename);
    return true;
}

void MainWindow::on_actionNew_triggered()
{
    if(showMessageBox())
    {
        qDebug() << "open new file";
        filename = "";
        ui->lineEditFile->setText("");
        ui->dcs->setNewCircuit();
        history.clear();
    }
}

bool MainWindow::showMessageBox()
{
    QMessageBox::StandardButton ack;
    ack = QMessageBox::information(this, "Save?", "Do you want to save this circuit?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if(ack == QMessageBox::Yes)
    {
        if(!on_actionSave_triggered())   return false;
        return true;
    }
    else if(ack == QMessageBox::No)
        return true;
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Ignore the event by default. otherwise the window will be closed always.
    event->ignore();
    if(showMessageBox())
        event->accept();
}

void MainWindow::on_actionCenter_On_triggered()
{
    ui->dcs->getCircuit()->centerOn();
}

void MainWindow::on_actionUndo_triggered()
{
    QPointF center_pos = ui->dcs->getCircuit()->getCenterPos();
    Circuit *cir = history.undo(ui->dcs->getCircuit());
    ui->dcs->setCircuit(cir);
    ui->dcs->centerOn(center_pos);
    cir->setCenterPos(center_pos);
    cir->init();
    if(mouse.isSeq())
    {
        ui->radioButtonSeq->click();
        ui->pageFF->setEnabled(true);
    }
    else
    {
        ui->radioButtonComb->click();
        ui->pageFF->setEnabled(false);
    }
}

void MainWindow::on_actionRedo_triggered()
{
    QPointF center_pos = ui->dcs->getCircuit()->getCenterPos();
    Circuit *cir = history.redo();
    ui->dcs->setCircuit(cir);
    ui->dcs->centerOn(center_pos);
    cir->setCenterPos(center_pos);
    cir->init();
    if(mouse.isSeq())
    {
        ui->radioButtonSeq->click();
        ui->pageFF->setEnabled(true);
    }
    else
    {
        ui->radioButtonComb->click();
        ui->pageFF->setEnabled(false);
    }
}

