#ifndef SEQININFO_H
#define SEQININFO_H

#include <QDialog>
#include "seqin.h"

namespace Ui {
class SeqInInfo;
}

class SeqInInfo : public QDialog
{
    Q_OBJECT

public:
    explicit SeqInInfo(QWidget *parent = nullptr);
    explicit SeqInInfo(SeqIn *seqin, QWidget *parent = nullptr);
    void changeWaveform();
    ~SeqInInfo();

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonSet_clicked();
    void on_comboBoxType_textActivated(const QString &arg1);
    void on_lineEditValue_textChanged(const QString &arg1);
    void on_checkBox_clicked();

private:
    Ui::SeqInInfo *ui;
    SeqIn *in;
};

#endif // SEQININFO_H
