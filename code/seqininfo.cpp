#include "seqininfo.h"
#include "ui_seqininfo.h"

QString tempValue;

SeqInInfo::SeqInInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeqInInfo)
{
    ui->setupUi(this);
}

SeqInInfo::SeqInInfo(SeqIn *combin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeqInInfo)
{
    in = combin;
    ui->setupUi(this);
    ui->lineEditName->setText(in->getName());
    ui->comboBoxType->setCurrentText(in->getValueType());
    on_comboBoxType_textActivated(in->getValueType());
    ui->lineEditValue->setText(in->getValue());
    tempValue = in->getValue();
    if(in->isDuplicate())
        ui->checkBox->setChecked(true);
    else
        ui->checkBox->setChecked(false);
    changeWaveform();
}

SeqInInfo::~SeqInInfo()
{
    delete ui;
}

void SeqInInfo::on_pushButtonCancel_clicked()
{
    close();
}


void SeqInInfo::on_pushButtonSet_clicked()
{
    in->setName(ui->lineEditName->text());
    in->setValue(ui->lineEditValue->text());
    in->setValueType(ui->comboBoxType->currentText());
    if(ui->checkBox->isChecked())
        in->setDuplicate(true);
    else
        in->setDuplicate(false);
    if(ui->comboBoxType->currentText() != "user-defined")
        in->text->setPlainText(ui->comboBoxType->currentText());
    else
        in->text->setPlainText(in->getName());
    close();
}


void SeqInInfo::on_comboBoxType_textActivated(const QString &arg1)
{
    if(arg1 == "user-defined")
    {
        ui->labelValue->setEnabled(true);
        ui->lineEditValue->setEnabled(true);
    }
    else
    {
        ui->labelValue->setEnabled(false);
        ui->lineEditValue->setEnabled(false);
        ui->checkBox->setChecked(true);
    }
    if(arg1 == "0")
        ui->lineEditValue->setText("0");
    else if(arg1 == "1")
        ui->lineEditValue->setText("1");
    else if(arg1 == "clock")
        ui->lineEditValue->setText("01");
    changeWaveform();
}

void SeqInInfo::on_checkBox_clicked()
{
    changeWaveform();
    if(!ui->checkBox->isChecked())
    {
        ui->comboBoxType->setCurrentText("user-defined");
        on_comboBoxType_textActivated("user-defined");
    }
}

void SeqInInfo::on_lineEditValue_textChanged(const QString &arg1)
{
    if(tempValue == arg1)   return;
    for(int i=0; i<arg1.length(); i++)
        if(arg1[i] != '0' && arg1[i] != '1')
        {
            ui->lineEditValue->setText(tempValue);
            return;
        }
    tempValue = arg1;
    changeWaveform();
}

void SeqInInfo::changeWaveform()
{
    QPixmap pixmap(170, 25);
    QPainter painter(&pixmap);
    QString str = ui->lineEditValue->text();
    if(!str.length())   return;
    if(ui->checkBox->isChecked())
    {
        while(str.length() < 10)
            str += ui->lineEditValue->text();
    }
    else
    {
        while(str.length() < 10)
            str += ui->lineEditValue->text().last(1);
    }
    pixmap.fill();
    str = '0' + str;
    for(int i=0; i<10; i++)
    {
        if(str[i+1] == '1')
            painter.drawLine(10+15*i, 5, 10+15*(i+1), 5);
        else
            painter.drawLine(10+15*i, 20, 10+15*(i+1), 20);
        if(str[i] != str[i+1])
            painter.drawLine(10+15*i, 5, 10+15*i, 20);
    }
    ui->labelWave->setPixmap(pixmap);
}
