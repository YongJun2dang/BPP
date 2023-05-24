#ifndef LMOCKMOTOR_H
#define LMOCKMOTOR_H
#include "lmotor.h"
class LMockMotor : public LMotor
{
public:
    LMockMotor();

    virtual int MoveInPulse(int axis, float velocity, float position);
    virtual int MoveRInPulse(int axis, float velocity, float distance) ;
    virtual int Stop(int axis);
    virtual int MoveCont(int axis, float velocity);
    virtual float GetPosition(int axis);
    virtual int IsMoving(int axis);


    virtual int OpenAll();
    virtual int Open(int axis);
    virtual int CloseAll();
    virtual int Close(int axis);

protected:
    float axis_data_position[128];
    int axis_in_position[128];
};

#endif // LMOCKMOTOR_H
