#include "ffinfo.h"
#include "ui_ffinfo.h"

FFInfo::FFInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FFInfo)
{
    ui->setupUi(this);
}

FFInfo::FFInfo(FlipFlop *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FFInfo)
{
    ff = f;
    ui->setupUi(this);
    ui->lineEditName->setText(ff->getName());
    ui->comboBoxType->setCurrentText(ff->getType());
    if(ff->getShowName())   ui->checkBox->setChecked(true);
    else                    ui->checkBox->setChecked(false);

    QVector<InNode *> inNodes = ff->getInNodes();
    QVector<OutNode *> outNodes = ff->getOutNodes();
    InNode *clk = ff->getClock();

    //show inputs
    for(InNode *in: qAsConst(inNodes))
        if(in->getConnect() && !in->getConnect()->getOutNodes().empty())
            ui->labelIn->setText(ui->labelIn->text() + in->getConnect()->getOutNodes().at(0)->getBelong()->getName() + "\n");

    //show outputs
    for(OutNode *out: qAsConst(outNodes))
        if(out->getConnect() && !out->getConnect()->getInNodes().empty())
            ui->labelOut->setText(ui->labelOut->text() + out->getConnect()->getInNodes().at(0)->getBelong()->getName() + "\n");

    //show clock
    if(clk->getConnect())
        ui->labelClock->setText(clk->getConnect()->getOutNodes().at(0)->getBelong()->getName() + "\n");
}

FFInfo::~FFInfo()
{
    delete ui;
}

void FFInfo::on_pushButtonCancel_clicked()
{
    close();
}


void FFInfo::on_pushButtonSet_clicked()
{
    ff->setName(ui->lineEditName->text());
    if(ui->checkBox->isChecked())   ff->setShowName(true);
    else                            ff->setShowName(false);

    if(ff->getType() != ui->comboBoxType->currentText())
    {
        FlipFlop *new_ff;
        if(ff->getInNodes().length() == 2)
        {
            if(ui->comboBoxType->currentText() == "DFF" || ui->comboBoxType->currentText() == "TFF")
                ff->subInNode();
        }
        else
        {
            if(ui->comboBoxType->currentText() == "JKFF" || ui->comboBoxType->currentText() == "RSFF")
                ff->addInNode();
        }

        switch (ui->comboBoxType->currentIndex() + DFF)
        {
        case DFF:   new_ff = new Dff (ff);   break;
        case RSFF:  new_ff = new RSff(ff);   break;
        case TFF:   new_ff = new Tff (ff);   break;
        case JKFF:  new_ff = new JKff(ff);   break;
        }
        ff->scene()->addItem(new_ff);
        ff->scene()->removeItem(ff);
        delete ff;
    }
    close();
}
