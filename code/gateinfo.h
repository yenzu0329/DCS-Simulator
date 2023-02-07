#ifndef GATEINFO_H
#define GATEINFO_H

#include <QDialog>
#include "and.h"
#include "or.h"
#include "nand.h"
#include "nor.h"
#include "xor.h"
#include "not.h"

namespace Ui {
class GateInfo;
}

class GateInfo : public QDialog
{
    Q_OBJECT

public:
    explicit GateInfo(QWidget *parent = nullptr);
    explicit GateInfo(Gate *gate, QWidget *parent = nullptr);
    ~GateInfo();

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonSet_clicked();
    void on_comboBoxType_textActivated(const QString &arg1);
    void on_spinBoxNum_valueChanged(int arg1);

private:
    Ui::GateInfo *ui;
    Gate *gate;
};

#endif // GATEINFO_H
