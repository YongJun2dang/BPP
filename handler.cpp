#include "handler.h"
#include "camerathread.h"
#include <QImage>
#include "iostream"

using namespace std;
Handler *handler = NULL;
LMotor *motor = NULL;

Handler::Handler(MainWindow* qmainWindow)
{
    handler = this;
    cameraGrabThread = new CameraGrabThread();
    cameraGrabThread->start();

    cameraThread = new CameraThread();
    connect(cameraThread, SIGNAL(sendImage(QImage)), qmainWindow, SLOT(changeLabel(QImage)));
    connect(cameraThread, SIGNAL(sendImageRight(QImage)), qmainWindow, SLOT(changeLabelRight(QImage)));

    cameraThread->start();

#ifdef USE_REAL_MOTOR
    motor = new REAL_MOTOR_CLASS ();
#else
    motor = new MOCK_MOTOR_CLASS ();
#endif

    motor->OpenAll();

    ShowProcessImage = 0;

    final_result.IsSuccessful = 0;
}


void Handler::Start()
{
    cameraThread->start_signal=true;
}

void Handler::Once()
{
    cameraThread->once_signal= true;
}

void Handler::Stop()
{
    cameraThread->stop_signal=true;
}

