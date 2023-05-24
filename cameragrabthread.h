#ifndef CAMERAGRABTHREAD_H
#define CAMERAGRABTHREAD_H

#include <QImage>
#include <QThread>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

class CameraGrabThread : public QThread
{
    Q_OBJECT
public:
    CameraGrabThread();
    Mat last_image;
    CvCapture *capture;
    int Grab();

private:
    void run();
    int exit_signal;
    void terminate();

};

#endif // CAMERAGRABTHREAD_H
