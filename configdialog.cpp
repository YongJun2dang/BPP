
#include "configdialog.h"
#include "ui_configdialog.h"

#include "handler.h"
#include <QMessageBox>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    //config set init value
    ui->HopperCountValue->setValue(5);
    ui->GripperDelayValue->setValue(10);
    ui->Option1Value->setValue(20);
    ui->Option2Value->setValue(33);

    for(int i=0;i<=handler->PICKUP_OPTION_MAX;i++)
    {
        ui->comboBoxPickupOption->addItem(QString::number(i));
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}


void ConfigDialog::on_buttonBox_accepted()
{
    hopperCount = QString::number(ui->HopperCountValue->value());
    gripperOnDelay = QString::number(ui->GripperDelayValue->value());
    option1 = QString::number(ui->Option1Value->value());
    option2 = QString::number(ui->Option2Value->value());
    QString qstr = ui->comboBoxPickupOption->currentText();

    QMessageBox msgBox;
    msgBox.setText(qstr);
    msgBox.exec();

    handler->PICKUP_OPTION = qstr.toInt();
}
