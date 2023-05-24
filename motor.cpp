#include "motor.h"
#include "ui_motor.h"
#include "motorunit.h"

Motor::Motor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Motor)
{
    ui->setupUi(this);
    int i;

    for(i=0;i<3;i++)
    {
        Add(i);
    }
}

Motor::~Motor()
{
    delete ui;
}
void Motor::Add(int index)
{
    QListWidgetItem *itemN = new QListWidgetItem();
    itemN->setSizeHint(QSize(1061, 38));
    ui->listWidget->addItem(itemN);
    MotorUnit* motor_unit = new MotorUnit();
    motor_unit->SetIndex(index);
    ui->listWidget->setItemWidget(itemN, motor_unit);
}
