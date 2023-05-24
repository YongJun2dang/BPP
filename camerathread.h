#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QImage>
#include <QThread>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;


class CameraThread : public QThread
{
    Q_OBJECT

public:
    CameraThread();
    int Grab();
    int Display(int is_idle = 0);
    int ProcessImage();
    int Pickup();
    int Place();
    bool stop_signal;
    bool start_signal;
    bool once_signal;
    Mat imgMat;
    int started_signal;

private:
    void run();



signals:
    void sendImage(QImage qimg);
    void sendImageRight(QImage qimg);

};
//extern CameraThread *cameraThread;


#endif // CAMERATHREAD_H
