#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "handler.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <iostream>
#include <QTimer>

#include "motor.h"
#include "io.h"

#include <fstream>
#include <string>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    handler = new Handler(this);
    ui->setupUi(this);

    //display summary on screen check
    std::ifstream file("/home/ubuntu/blister.txt");
    std::string str;
    std::getline(file, str);
    if(str=="DISPLAY_SUMMARY_ON_SCREEN = 1")
    {
        ui->lineEditSummaryOnScreen->setVisible(true);
    }else
    {
        ui->lineEditSummaryOnScreen->setVisible(false);
    }
    ui->lineEditSummaryOnScreen->setText("recognition rate : 100%");


    //quit application
    QObject::connect(ui->buttonExit, SIGNAL(clicked()),
                     QApplication::instance(), SLOT(quit()));


    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    if (timer->isActive())
            timer->stop();

    delete ui;
}


void MainWindow::onTimer()
{
    if(handler->final_result.IsSuccessful==0)
    {
        ui->label_data->setText(
                "Not found");
    }
    else
    {
        ui->label_data->setText(
                    "Found, Length: " +
                    QString::number(handler->final_result.Length) + ","
                    "X: " + QString::number(handler->final_result.pivot_data.X) +","
                    "Y: " + QString::number(handler->final_result.pivot_data.Y) +","
                    "Image Processing: " + QString::number(handler->final_result.processing_time) +","
                    );
    }
}
void MainWindow::changeLabel(QImage qimg)
{
    ui->label->setPixmap(QPixmap::fromImage(qimg).scaled(ui->label->size()));
}
void MainWindow::changeLabelRight(QImage qimg)
{
    ui->label_2->setPixmap(QPixmap::fromImage(qimg).scaled(ui->label_2->size()));
}


void MainWindow::on_buttonStart_clicked()
{
    if(handler==NULL)
    {
        std::cout<<"NULL Error";
    }
    else
    {
        handler->Start();
    }

    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setEnabled(false);
}


void MainWindow::on_buttonOnce_clicked()
{
    handler->Once();
}

void MainWindow::on_buttonStop_clicked()
{
    handler->Stop();
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setEnabled(false);
}

void MainWindow::on_buttonConfig_clicked()
{
    ConfigDialog configDialog(this);
    configDialog.setModal(true);
    if(configDialog.exec() == QDialog::Accepted){
        //data
        QString data = "HopperCount: "+configDialog.hopperCount+
                ", GripperOnDelay: "+configDialog.gripperOnDelay+
                ", Option1: "+configDialog.option1+
                ", Option2: "+configDialog.option2;

        //config values save in file directory is where the application in
        QFile file(QApplication::applicationDirPath()+"/config.txt");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        QTextStream out(&file);
        out << data;
        file.close();
    }
}


void MainWindow::on_buttonExit_clicked()
{

}

void MainWindow::on_pushButtonMotors_clicked()
{
   Motor *motorui = new Motor();
   motorui->show();
}

void MainWindow::on_pushButtonIOs_clicked()
{
    IO *ioui = new IO();
    ioui->show();
}

void MainWindow::on_checkBoxProcessImage_stateChanged(int arg1)
{
    if(arg1==0)
    {
        handler->ShowProcessImage = 0;
    }
    else
    {
        handler->ShowProcessImage = 1;
    }

}
