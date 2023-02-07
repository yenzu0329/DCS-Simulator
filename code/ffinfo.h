#ifndef FFINFO_H
#define FFINFO_H

#include <QDialog>
#include "dff.h"
#include "tff.h"
#include "jkff.h"
#include "rsff.h"

namespace Ui {
class FFInfo;
}

class FFInfo : public QDialog
{
    Q_OBJECT

public:
    explicit FFInfo(QWidget *parent = nullptr);
    explicit FFInfo(FlipFlop *ff, QWidget *parent = nullptr);
    ~FFInfo();

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonSet_clicked();

private:
    Ui::FFInfo *ui;
    FlipFlop *ff;
};

#endif // FFINFO_H
