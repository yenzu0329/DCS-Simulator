#include "gateinfo.h"
#include "ui_gateinfo.h"

GateInfo::GateInfo(QWidget *parent):
    QDialog(parent),
    ui(new Ui::GateInfo)
{
    ui->setupUi(this);
}

GateInfo::GateInfo(Gate *g, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GateInfo)
{
    gate = g;
    ui->setupUi(this);
    ui->lineEditName->setText(gate->getName());
    ui->comboBoxType->setCurrentText(gate->getType());
    ui->spinBoxNum->setValue(gate->getInNodes().length());
    if(gate->getType() == "NOT")    ui->spinBoxNum->setEnabled(false);
    if(gate->getShowName())         ui->checkBox->setChecked(true);
    else                            ui->checkBox->setChecked(false);

    QVector<InNode *> inNodes = gate->getInNodes();
    OutNode *out = gate->getOutNodes().at(0);

    //show inputs
    for(InNode *in: qAsConst(inNodes))
        if(in->getConnect() && !in->getConnect()->getOutNodes().empty())
            ui->labelIn->setText(ui->labelIn->text() + in->getConnect()->getOutNodes().first()->getBelong()->getName() + "\n");

    //show outputs
    if(out->getConnect())
    {
        inNodes = out->getConnect()->getInNodes();
        for(InNode *in: qAsConst(inNodes))
            ui->labelOut->setText(ui->labelOut->text() + in->getBelong()->getName() + "\n");
    }
}

GateInfo::~GateInfo()
{
    delete ui;
}

void GateInfo::on_pushButtonCancel_clicked()
{
    close();
}


void GateInfo::on_pushButtonSet_clicked()
{
    gate->setName(ui->lineEditName->text());
    if(ui->checkBox->isChecked())   gate->setShowName(true);
    else                            gate->setShowName(false);

    if(gate->getType() != ui->comboBoxType->currentText())
    {
        Gate *new_gate;
        switch (ui->comboBoxType->currentIndex())
        {
        case AND:   new_gate = new And(gate);   break;
        case OR:    new_gate = new Or (gate);   break;
        case NAND:  new_gate = new Nand(gate);  break;
        case NOR:   new_gate = new Nor(gate);   break;
        case NOT:   new_gate = new Not(gate);   break;
        case XOR:   new_gate = new Xor(gate);   break;
        }
        gate->scene()->addItem(new_gate);
        gate->scene()->removeItem(gate);
        delete gate;
        gate = new_gate;
    }

    while(gate->getInNodes().length() > ui->spinBoxNum->value())
        gate->subInNode();
    while(gate->getInNodes().length() < ui->spinBoxNum->value())
        gate->addInNode();

    close();
}


void GateInfo::on_comboBoxType_textActivated(const QString &arg1)
{
    if(arg1 == "NOT")
    {
        ui->spinBoxNum->setValue(1);
        ui->spinBoxNum->setEnabled(false);
    }
    else
    {
        if(ui->spinBoxNum->value() == 1)
            ui->spinBoxNum->setValue(2);
        ui->spinBoxNum->setEnabled(true);
    }
}


void GateInfo::on_spinBoxNum_valueChanged(int arg1)
{
    if(arg1 > 1) return;
    if(ui->comboBoxType->currentText() != "NOT")
        ui->spinBoxNum->setValue(2);
}

