#include "motorunit.h"
#include "ui_motorunit.h"
#include "iostream"

MotorUnit::MotorUnit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorUnit)
{
    ui->setupUi(this);
}

MotorUnit::~MotorUnit()
{
    delete ui;
}

void MotorUnit::SetIndex(int index)
{
    this->index = index;
    ui->labelIndex->setText(QString::number(index));

    switch(index){
    case 0:
        ui->lineEditMotorNum->setText("MT_X");
        break;
    case 1:
        ui->lineEditMotorNum->setText("MT_Y");
        break;
    case 2:
        ui->lineEditMotorNum->setText("MT_Z");
        break;
    }
}

int MotorUnit::GetIndex()
{
    return index;
}

void MotorUnit::on_pushButtonOn_clicked()
{
    motor->MoveRInPulse(index, 100, 100);
}

void MotorUnit::on_pushButtonPosSlow_pressed()
{
    motor->MoveCont(index, 100);
//    motor -> GetPosition(index);
//    motor->YongReadData(index);
}

void MotorUnit::on_pushButtonPosSlow_released()
{
    motor->Stop(index);
}


void MotorUnit::on_pushButtonPosFast_pressed()
{
    motor->MoveCont(index, 2000);
}

void MotorUnit::on_pushButtonPosFast_released()
{
    motor->Stop(index);
}


void MotorUnit::on_pushButtonNegSlow_pressed()
{
    motor->ReverseMoveCont(index,100);
}

void MotorUnit::on_pushButtonNegSlow_released()
{
    motor->Stop(index);
}

void MotorUnit::on_pushButtonNegFast_pressed()
{
    motor->ReverseMoveCont(index,2000);
}

void MotorUnit::on_pushButtonNegFast_released()
{
    motor->Stop(index);
}


void MotorUnit::on_pushButtonHMinus_clicked()
{
//    motor -> GetPosition(index);
//    motor->YongReadData(index);
}

void MotorUnit::on_pushButtonMove_clicked()
{
    float distance = ui->lineEditMotorInputData->text().toInt();
    motor->MoveRInPulse(index, 100, distance);
}

void MotorUnit::on_pushButtonReset_clicked()
{
    motor->MoveInPulse(index,0,0);
}
