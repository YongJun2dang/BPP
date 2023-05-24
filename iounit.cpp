#include "iounit.h"
#include "ui_iounit.h"
#include "iostream"
IOUnit::IOUnit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IOUnit)
{
    ui->setupUi(this);
    ui->pushButtonInput->setStyleSheet("background-color: white");
    ui->pushButtonOutput->setStyleSheet("background-color: lightgray");
}

IOUnit::~IOUnit()
{
    delete ui;
}

void IOUnit::SetIndex(int index)
{
    this->index = index;
    ui->pushButtonInput->setText(QString::number(index));
    ui->pushButtonOutput->setText(QString::number(index));
}

int IOUnit::GetIndex()
{
    return index;
}

void IOUnit::on_pushButtonOutput_clicked()
{
    std::cout<<index<<std::endl;
    ChangeInputColor(1);
}

void IOUnit::ChangeInputColor(int flag)
{
    if(flag){
        ui->pushButtonInput->setStyleSheet("background-color: yellow");
    }else{
        ui->pushButtonInput->setStyleSheet("background-color: white");
    }
}
