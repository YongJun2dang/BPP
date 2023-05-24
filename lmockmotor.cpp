#include "lmockmotor.h"
#include <stdlib.h>
#include<memory.h>
#include<stdio.h>
LMockMotor::LMockMotor()
{
    memset(axis_data_position,0,sizeof(axis_data_position));
    memset(axis_in_position,0,sizeof(axis_in_position));
}


// absoluve movement
int LMockMotor::MoveInPulse(int axis, float velocity, float position)
{
    printf("MoveInPulse axis: %d, velocity: %.3f, position: %.3f\n",axis, velocity, position);
    axis_in_position[axis] = 1;
}

int LMockMotor::MoveRInPulse(int axis, float velocity, float distance)
{
    printf("MoveRInPulse axis: %d, velocity: %.3f, distance: %.3f\n",axis, velocity, distance);

    axis_in_position[axis] = 1;
}
int LMockMotor::Stop(int axis)
{
    printf("Stop axis: %d", axis);
    axis_in_position[axis] = 1;
}

int LMockMotor::MoveCont(int axis, float velocity)
{
    printf("MoveCont axis: %d", axis);
    axis_in_position[axis] = 1;
}

float LMockMotor::GetPosition(int axis)
{
    if(axis>=0 && axis<128)
    {
        return axis_data_position[axis];
    }
    return 0.0;
}

int LMockMotor::IsMoving(int axis)
{
    if(axis>=0 && axis<128)
    {
        int retv = axis_in_position[axis];
        axis_in_position[axis] = 0;
        return retv;
    }
    return 0;
}

int LMockMotor::OpenAll()
{
    return 0;
}
int LMockMotor::Open(int axis)
{
    return 0;
}

int LMockMotor::CloseAll()
{
    return 0;
}

int LMockMotor::Close(int axis)
{
    return 0;
}
