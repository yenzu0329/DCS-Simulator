#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "dockwidget.h"
#include "table.h"
#include "mouse.h"
#include "selectbox.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setButton();
    void setToolBars();
    void setActionEnable(bool);
    void buttonGroupClicked(int id);
    bool showMessageBox();

private slots:
    void changeMode();
    void showSimul();
    void closeEvent(QCloseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void on_radioButtonComb_clicked();
    void on_radioButtonSeq_clicked();
    void on_actionDelete_triggered();
    void on_actionWire_triggered();
    void on_actionSelect_triggered();
    void on_pushButtonSimul_clicked();
    void on_actionRun_triggered();
    void on_actionStop_triggered();
    void on_lineEditSearch_returnPressed();
    void on_toolButtonSearch_clicked();
    void on_actionOpen_triggered();
    bool on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionCenter_On_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

private:
    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup;
    DockWidget *dockwidget;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *simulToolBar;
    QString filename;
};
#endif // MAINWINDOW_H
