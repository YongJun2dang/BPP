#include "lmotor.h"
#include <stdio.h>
LMotor::LMotor()
{
}

void LMotor::SetName(char* name)
{
    sprintf(buf_name, "%s", name);
}
