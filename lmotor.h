#ifndef LMOTOR_H
#define LMOTOR_H

class LMotor
{
public:
    LMotor();
    void SetName(char* name);
    // absoluve movement
    virtual int MoveInPulse(int axis, float velocity, float position) = 0;
    virtual int MoveRInPulse(int axis, float velocity, float distance) = 0;
    virtual int Stop(int axis) = 0;
    virtual int MoveCont(int axis, float velocity) = 0;
    virtual float GetPosition(int axis) = 0;
    virtual int IsMoving(int axis) = 0;
    virtual int OpenAll() = 0;
    virtual int Open(int axis) = 0;
    virtual int CloseAll() = 0;
    virtual int Close(int axis) = 0;
    virtual int ReverseMoveCont(int axis, float velocity) = 0;
    virtual int ReverseMoveRInPulse(int axis, float velocity, float distance) = 0;
    virtual int MotorConfigure(int axis,int accuracy) = 0;
    virtual int MotorCalc(int axis, float velocity, float distance) = 0;
    virtual int YongReadData(int usbNumber) = 0;

protected:
    char buf_name[128];
};

#endif // LMOTOR_H
