#include "camerathread.h"
#include <QMessageBox>
#include <iostream>
#include <QImage>
#include <QVector>
#include "handler.h"
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

using namespace cv;
using namespace std;

bool cmp(const Coord2D &data1, const Coord2D &data2)
{
    float center_x = handler->CAM_DATA_WIDTH/2;
    float center_y = handler->CAM_DATA_HEIGHT/2;

    float dist1;
    float dist2;

    dist1 = pow(data1.X - center_x, 2) + pow(data1.Y - center_y, 2);
    dist2 = pow(data2.X - center_x, 2) + pow(data2.Y - center_y, 2);
    if(handler->PICKUP_OPTION==1 ||
       handler->PICKUP_OPTION==0)
        return dist1 > dist2;
    else if(handler->PICKUP_OPTION==2)
        return dist2 > dist1;
    return dist2 > dist1;
}
/*
int CameraThread::compaerValue()
{
    vector<pair<float,float>> compare;
    compare.back(pair<float,float>(dist1,dist2));

}
*/



CameraThread::CameraThread()
{
    start_signal = false;
    stop_signal = false;
    once_signal = false;

    started_signal = false;
}

int CameraThread::Display(int is_idle)
{
    // is_idle 1: if running, 0: if idle
    Mat imgMat1 = handler->cameraGrabThread->last_image;
    if(imgMat1.type()==CV_8UC1){
        //cout<< "CV_8UC1" <<endl;
        QVector<QRgb> sColorTable;
        if(sColorTable.isEmpty())
        {    sColorTable.resize(256);
            for(int i=0; i<256; ++i)
            {
                sColorTable[i] = qRgb(i, i, i);
            }
        }

        // casting to QImage
        QImage qimg((uchar*)imgMat1.data, imgMat1.cols, imgMat1.rows, imgMat1.step, QImage::Format_Indexed8);
        // if(!handler->ShowProcessImage) // if the checkbox is unchecked
        // {
            emit sendImage(qimg);
        // }
    }
    else if(imgMat1.type()==CV_8UC3)
    {
        // if(!handler->ShowProcessImage) // if the checkbox is unchecked
        // {
            QImage qimg((uchar*)imgMat1.data, imgMat1.cols, imgMat1.rows, imgMat1.step, QImage::Format_RGB888);
            emit sendImage(qimg.rgbSwapped());
        // }
    }

}

int CameraThread::ProcessImage()
{
    JimmyResult jimmy_result;
    double time_started = GetTickCount();

    jimmy_result.IsSuccessful = 0;

    // JIMMY : draw process image on the raw image
    //Mat drawing = imgMat;
    Mat drawing = Mat::zeros(imgMat.size(), CV_8UC3);

    // JIMMY : Canny auto
    if(CANNY_AUTO_THRESHOLD)
    {
        Canny(imgMat, imgMat, CANNY_AUTO_MIN, CANNY_AUTO_MAX, 5);
    }
    else
    {
        Canny(imgMat, imgMat, CANNY_AUTO_MIN, CANNY_AUTO_MAX, 5);
    }

    std::vector<std::vector<cv::Point> > contours;
    findContours(imgMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<Moments> mu(contours.size());

    for (int i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i], false);
    }
    //  Get the mass centers:
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
    }
    //contours to point iterator
    std::vector<std::vector<cv::Point> >::iterator it = contours.begin();
    int i = 0;
    //change color gray -> CV_8UC3


    //draw line and center
    float pointX = 0;
    float pointY = 0;


    int count_found_pattern = 0;


    std::vector<Coord2D> map_data;

    while (it != contours.end())
    {
        //poly vector
        std::vector<cv::Point> poly;
        //get poly vector from points (poly line length)etect unspread packs
        if (poly.size() > 4 && fabs(contourArea(Mat(poly))) > 1000)
        {
            //draw lines
            polylines(drawing, poly, true, Scalar(0, 0, 255), 5);
            //draw centers
            circle(drawing, mc[i], 4, Scalar(0, 0, 255), -1, 8, 0);

            cv::string str = "X";
            putText(drawing,str,mc[i],0,0.5,Scalar::all(200));

        }
        cv::approxPolyDP(*it, poly, 10, true);
        //is it square?
        // if (poly.size() >= 4)
        if (poly.size() == 4 && fabs(contourArea(Mat(poly))) > 1000 && isContourConvex(Mat(poly)))
        {
            //draw lines
            cv:polylines(drawing, poly, true, Scalar(0, 255, 0), 5);
            //draw centers
            circle(drawing, mc[i], 4, Scalar(100, 200, 0), -1, 8, 0);
            char buf[16];
            sprintf(buf,"%d",count_found_pattern+1);
            cv::string str = cv::string(buf);

            Coord2D c2d;
            c2d.X = mc[i].x;
            c2d.Y = mc[i].y;
            map_data.push_back(c2d);

            count_found_pattern++;

            if(pointX==0&&pointY==0)
            {
                pointX=mc[i].x;
                pointY=mc[i].y;
            }
        }
        /*
        // JIMMY : detect unspread packs
        else if (poly.size() > 4 && fabs(contourArea(Mat(poly))) > 1000)
        {
            //draw lines
            polylines(drawing, poly, true, Scalar(0, 0, 255), 5);
            //draw centers
            circle(drawing, mc[i], 4, Scalar(0, 0, 255), -1, 8, 0);

            cv::string str = "X";
            putText(drawing,str,mc[i],0,0.5,Scalar::all(200));

        }*/
        ++it;
        i++;
    }

     std::sort(map_data.begin(), map_data.end(), cmp);

     int myturn = 0;
     for(vector<Coord2D>::iterator itor = map_data.begin();
        itor!= map_data.end();
        itor++)
     {
         char buf[16];
         sprintf(buf,"%d",++myturn);
         cv::string str = cv::string(buf);

         Point2f pts = Point2f(itor-> X, itor->Y);
         putText(drawing,str,pts,0,0.5,Scalar::all(200));
     }
    if(count_found_pattern>0)
    {
        jimmy_result.IsSuccessful = 1;
        jimmy_result.Length = count_found_pattern;
        jimmy_result.pivot_data = map_data[0];
        jimmy_result.map_all_data = map_data;
    }
    QImage qimg_display((uchar*)drawing.data, drawing.cols, drawing.rows, drawing.step, QImage::Format_RGB888);
    emit sendImageRight(qimg_display.rgbSwapped());
    /*
    if(handler->ShowProcessImage)
    {
        emit sendImage(qimg_display.rgbSwapped());
    }*/

    ostringstream osX;
    osX<<pointX;
    ostringstream osY;
    osY<<pointY;
    if(pointX!=0&&pointY!=0){
        cout<<"X: "+osX.str()+", Y: "+osY.str()<<endl;
    }


    jimmy_result.processing_time = GetTickCount() - time_started;

    // overwrite the final image
    handler->final_result = jimmy_result;
    return 0;
}

int CameraThread::Pickup()
{
    // Move to pickup position
    // motor->MoveInPulse(MT_X, handler->pos[POS_PICKUP].X, 100);
    motor->MoveRInPulse(MT_X, 100, 100);
    usleep(1000*1000);    // check motors

    // move motor
   /// MoveXYPosition(POS_PICKUP);

   /// MoveZToPickupLevel();

   /// GripForPickupPosition();

   /// MoveZToOffsetLevel();

    return 0;
}
int CameraThread::Place()
{
    int retv;
    motor->MoveRInPulse(MT_Y, 100, 100);
    usleep(1000*1000);
    if(1)
    {
        // Okay
        /// handler->YIELD_OKAY++;
    }
    else
    {
        /// handler->YIELD_NG++;
    }
    return 0;
}

void CameraThread::run()
{
    stop_signal=false;

    while(1)
    {
        usleep(1*1000);
        if(handler==NULL) continue;
        if(handler->cameraGrabThread==NULL) continue;
        if(handler->cameraGrabThread->last_image.empty()) continue;

        // Grab everyday and everytime
        // Grab();
        imgMat = handler->cameraGrabThread->last_image;
        // Handle for click of start button
        int error = false;
        if(start_signal || once_signal)
        {
            if(!error && Display())
            {
                // sth wrong wrt Grabbing
            }
            if(!error && ProcessImage())
            {
            }

            if(!handler->final_result.IsSuccessful)
            {
                QMessageBox msgBox;
                msgBox.setText("Not found");
                msgBox.exec();
                error = 1;
            }


            if(!error && Pickup())
            {
            }
            if(!error && Place())
            {
                motor->MoveRInPulse(1, 100, 100);
                usleep(1000*10);
            }
            once_signal = false;

            if(error)
            {
                start_signal = false;
                once_signal = false;
            }
        }
        else
        {
            Display();
        }


        // Handle for click of stop button
        if(stop_signal)
        {
            start_signal = 0;
            stop_signal = 0;
        }
        usleep(1000);
    }
}

