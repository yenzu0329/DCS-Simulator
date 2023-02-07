#ifndef COMBININFO_H
#define COMBININFO_H

#include <QDialog>
#include "combin.h"

namespace Ui {
class CombInInfo;
}

class CombInInfo : public QDialog
{
    Q_OBJECT

public:
    explicit CombInInfo(QWidget *parent = nullptr);
    explicit CombInInfo(CombIn *combin, QWidget *parent = nullptr);
    ~CombInInfo();

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonSet_clicked();

private:
    Ui::CombInInfo *ui;
    CombIn *in;
};

#endif // COMBININFO_H
