#include "cameragrabthread.h"
#include <QMessageBox>
#include <iostream>
#include <QImage>
#include <QVector>
#include "handler.h"

CameraGrabThread::CameraGrabThread()
{
    exit_signal = 0;
}


int CameraGrabThread::Grab()
{
    // raw image
    Mat img;
    img = cvQueryFrame(capture);
    if(!img.empty())
    {
        last_image = img;
    }
    return 0;
}

void CameraGrabThread::run()
{

    capture = NULL;
    handler->CAM_DATA_ASSIGNED = 0;
    capture = cvCaptureFromCAM(CV_CAP_ANY);
    if(capture==NULL)
    {
        QMessageBox msgBox;
        msgBox.setText("Camera Open Failed");
        msgBox.exec();
        return;
    }
    handler->CAM_DATA_HEIGHT = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    handler->CAM_DATA_WIDTH = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    handler->CAM_DATA_FPS = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    handler->CAM_DATA_ASSIGNED = 1;


    while(1)
    {
        Grab();
        usleep(1000);
        if(exit_signal) break;
    }
}

void CameraGrabThread::terminate()
{
    cvReleaseCapture(&capture);
}
