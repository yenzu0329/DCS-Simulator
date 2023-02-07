#include "combininfo.h"
#include "ui_combininfo.h"

CombInInfo::CombInInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombInInfo)
{
    ui->setupUi(this);
}

CombInInfo::CombInInfo(CombIn *combin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombInInfo)
{
    in = combin;
    ui->setupUi(this);
    ui->lineEditName->setText(in->getName());
    ui->comboBoxType->setCurrentIndex(in->getValue().type());
}

CombInInfo::~CombInInfo()
{
    delete ui;
}

void CombInInfo::on_pushButtonCancel_clicked()
{
    close();
}

void CombInInfo::on_pushButtonSet_clicked()
{
    in->setName(ui->lineEditName->text());
    in->setValue(ui->comboBoxType->currentIndex());
    if(in->getValue().type() == UNKNOWN)
        in->text->setPlainText(in->getName());
    else if(in->getValue().type() == ONE)
        in->text->setPlainText("1");
    else
        in->text->setPlainText("0");
    close();
}

