#ifndef HANDLER_H
#define HANDLER_H

#include "mainwindow.h"
#include "camerathread.h"
#include "cameragrabthread.h"
#include "lmotor.h"
#include "lmockmotor.h"
#include "lyongmotor.h"

#define USE_REAL_MOTOR

#define MOCK_MOTOR_CLASS LMockMotor
#define REAL_MOTOR_CLASS LYongMotor

#define MT_X 0
#define MT_Y 1
#define MT_Z 2

#define POS_PICKUP 0
#define POS_PLACE 1
#define POS_INSPECTION 2

#define MOTOR_X_RPM_MAX 3000
#define MOTOR_Y_RPM_MAX 3000
#define MOTOR_Z_RPM_MAX 3000

#define MOTOR_X_CYCLE_PULSE 12
#define MOTOR_Y_CYCLE_PULSE 12
#define MOTOR_Z_CYCLE_PULSE 12

#define MOTOR_X_CYCLE_DISTANCE 0
#define MOTOR_Y_CYCLE_DISTANCE 37.7 //(mm)
#define MOTOR_Z_CYCLE_DISTANCE 25.1 //(mm)

#define CANNY_AUTO_THRESHOLD 0
#define CANNY_AUTO_MIN 50
#define CANNY_AUTO_MAX 225


typedef struct _Coord3D {
    float X;
    float Y;
    float Z;

} Coord3D;

typedef struct _Coord2D {
    float X;
    float Y;
} Coord2D;

typedef struct _BlisterPos {
    float X;
    float Y;
    int Pickable; //1: pickup possible(SolelyFlag=1&&PositionStyle=1), 0: impossible
    int SolelyFlag; //1: solely 0: group
    float angle; //radian
    int PositionStyle; //1: up(can see medicine 0: down(can not see medicine) 2: else

} BlisterPos;



typedef struct _JimmyResult
{
    int IsSuccessful; // success if 1
    int Length;
    Coord2D pivot_data;
    std::vector<Coord2D> map_all_data;
    double processing_time;
    int Shakable; //1: should shake
    int Feedable; //1: should feed more blister
} JimmyResult;


class Handler : public QObject
{
public:

    Handler(MainWindow*);
    CameraGrabThread* cameraGrabThread;
    CameraThread *cameraThread;
    void Start();
    void Stop();
    void Once();
public:
    int ShowProcessImage;


public:
    // hardware information variables
    int CAM_DATA_WIDTH;
    int CAM_DATA_HEIGHT;
    int CAM_DATA_FPS;
    int CAM_DATA_ASSIGNED; // whether variables above are assigned

public:

    // global purpose variables

    // 0: OPTIMIZED, 1: CENTER FIRST, 2: OUTER FIRST
    int PICKUP_OPTION;
    static const int PICKUP_OPTION_MAX = 2;

    int YIELD_OKAY;
    int YIELD_NG;
    Coord3D pos[16];
    JimmyResult final_result;
};
extern Handler *handler;
extern LMotor *motor;


#endif // HANDLER_H
