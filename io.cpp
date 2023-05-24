#include "io.h"
#include "ui_io.h"
#include "iounit.h"


IO::IO(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IO)
{
    ui->setupUi(this);
    int i;
    for(i=0;i<8;i++)
    {
        Add(i);
    }
}

IO::~IO()
{
    delete ui;
}

void IO::Add(int index)
{
    QListWidgetItem *itemN = new QListWidgetItem();
    itemN->setSizeHint(QSize(278, 28));
    ui->listWidget->addItem(itemN);
    IOUnit* io_unit = new IOUnit();
    io_unit->SetIndex(index);
    ui->listWidget->setItemWidget(itemN, io_unit);
}
