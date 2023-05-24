#include "lyongmotor.h"
#include <QApplication>
#include <stdio.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"
#include "iostream"
#include <poll.h>
#include <QtWidgets>
#include <errno.h>

LYongMotor::LYongMotor()
{
    int i;
    for(i=0;i<3;i++)
    {
        axisToUsbNumber[i] = -1;
    }
    for(i=0;i<LengthUsbNo;i++)
    {
        usbNoToFileDesc[i] = -1;
    }
    gpio_set_value();
}

// https://stackoverflow.com/questions/5877504/access-c-queue-elements-like-an-array
void LYongMotor::run()
{
    while(true)
    {
        for(int i=0;i<LengthUsbNo;i++)
        {
            UpdateFromSerial(i);
            ParsingSerial(i);
        }
        usleep(50);

    }

}
int LYongMotor::UpdateFromSerial(int usbNo)
{
    int fd = usbNoToFileDesc[usbNo];
    if(fd<0) return 0;

    char buf[128];
    int n = read(fd, buf, 128);
    if(n<=0) return 0;
    for(int i=0;i<n;i++)
    {
        myQueue[usbNo].push(buf[i]);
    }
    return 0;
}
int LYongMotor::ParsingSerial(int usbNo)
{
    // TODO:
    // 향후 시리얼 통신 규격에 있어 완전히 파악하면 이걸 사용하자

    return 0;
    while(true)
    {
        int n = myQueue[usbNo].size();

        if(n>0)
        {
            n = n + 0;
            n = myQueue[usbNo].size();
        }
        if(n>=7 && myQueue[usbNo].c[3]==133)
        {
            // 184 183 101 7 1 149 143
            char id = myQueue[usbNo].c[2];

            if(id==(char)100)
            {
                axisToUsbNumber[0] = usbNo;
            }
            else if(id==(char)101)
            {
                axisToUsbNumber[1] = usbNo;
            }
            else if(id==(char)102)
            {
                axisToUsbNumber[2] = usbNo;
            }
            for(int i=0;i<7;i++)
            {
                myQueue[usbNo].pop();
            }
        }
    }

    return 0;
}


int LYongMotor::WriteMotorDataByUsb(int usbno, char* command, int len)
{
    int i;
    char byChkSend = 0;
    for(i=0;i<len-1;i++)
    {
        byChkSend += command[i];
    }
    byChkSend = (~byChkSend) + 1;
    command[len-1] = byChkSend;


    int myfd = usbNoToFileDesc[usbno];
    if(myfd<0)
    {
        return 1; // fail
    }
    int retv = write(myfd, command, len);

    return 0;

}

int LYongMotor::WriteMotorData(int axis, char* command, int len)
{
    int i;
    char byChkSend = 0;
    for(i=0;i<len-1;i++)
    {
        byChkSend += command[i];
    }
    byChkSend = (~byChkSend) + 1;
    command[len-1] = byChkSend;

    int myfd = GetFileDescriptor(axis);
    if(myfd<0)
    {
        return 1; // fail
    }
    int retv = write(myfd, command, len);

    return 0;
}


// absoluve movement
int LYongMotor::MoveInPulse(int axis, float velocity, float position)
{
    int motor_axis = axis + 100;
    char command[] = {
        183
        , 184
        , (char)motor_axis
        , 219
        , 6
        , position //position
        , 0
        , 0
        , 0
        , velocity //velocity(default min 0, max 255) 1000rpm
        , 0
        , 0 // checksum
    };

    WriteMotorData(axis, command, sizeof(command));
    return 0;
}
// absoluve movement (Reverse)
int LYongMotor::ReverseMoveInPulse(int axis, float velocity, float position)
{
    int motor_axis = axis + 100;
    char command[] = {
        183
        , 184
        , (char)motor_axis
        , 219
        , 6
        , position //position
        , 0
        , 0
        , 0
        , velocity //velocity(default min 0, max 255) 1000rpm
        , 0
        , 0 // checksum
    };

    WriteMotorData(axis, command, sizeof(command));
    return 0;
}

//relative
int LYongMotor::MoveRInPulse(int axis, float velocity, float distance)
{
    int motor_axis = axis + 100;
    int distance_quotient = (int)distance/256;
    int distance_remainder = (int)distance%256;
    int velocity_quotient = (int)velocity/256;
    int velocity_remainder = (int)velocity%256;

    char command[] = {
        183
        , 184
        , (char)motor_axis
        , 220
        , 6
        , distance_remainder
        , distance_quotient
        , 0
        , 0
        , velocity_remainder
        , velocity_quotient
        , 0 // checksum
    };
    WriteMotorData(axis, command, sizeof(command));
    return 0;
}

//relative(Reverse)
int LYongMotor::ReverseMoveRInPulse(int axis, float velocity, float distance)
{

    int motor_axis = axis + 100;
    int distance_quotient = 255 - (int)distance/256;
    int distance_remainder = 256 -(int)distance%256;
    int velocity_quotient = 255 - (int)velocity/256;
    int velocity_remainder = 256 -(int)velocity%256;
    char command[] = {
        183
        , 184
        , (char)motor_axis
        , 220
        , 6
        , distance_remainder
        , distance_quotient
        , 255
        , 255
        , velocity_remainder
        , velocity_quotient
        , 0 // checksum
    };
    WriteMotorData(axis, command, sizeof(command));
    return 0;
}
int LYongMotor::Stop(int axis)
{
    int myfd = GetFileDescriptor(axis);
    if(myfd<0)
    {
        return 1; // fail
    }

    int motor_axis = axis + 100;

    char stop_1[] = {
        0xb7,
        0xb8,
        (char)motor_axis,
        130,
        2,
        0,
        0,
        79  // checksum

    };
    WriteMotorData(axis, stop_1, sizeof(stop_1));



    return 0;
}

// jog
int LYongMotor::MoveCont(int axis, float velocity)
{
    int myfd = GetFileDescriptor(axis);
    if(myfd<0)
    {
        return 1; // fail
    }

    int motor_axis = axis + 100;
    int quotient = (int)velocity/256;
    int remainder = (int)velocity%256;
    char start_1[] = {
        0xb7
        ,0xb8
        ,(char)motor_axis
        ,130
        ,2
        ,remainder
        ,quotient
        ,0
    };
    WriteMotorData(axis, start_1, sizeof(start_1));
    return 0;
}

// reverse jog
int LYongMotor::ReverseMoveCont(int axis, float velocity)
{
    int myfd = GetFileDescriptor(axis);
    if(myfd<0)
    {
        return 1; // fail
    }
    int motor_axis = axis + 100;

    int quotient = 255 - (int) velocity/256;
    int remainder = 256 - (int) velocity%256;

    char start_1[] = {
        0xb7
        ,0xb8
        ,(char)motor_axis
        ,130
        ,2
        ,remainder
        ,quotient
        ,0
    };
    WriteMotorData(axis, start_1, sizeof(start_1));
    return 0;
}

int LYongMotor::IsMoving(int axis)
{
    int myfd = GetFileDescriptor(axis);
    return 0;
}
int LYongMotor::GetFileDescriptor(int axis)
{
    if(axisToUsbNumber[axis]<0) return -1;
    int usbNumber = axisToUsbNumber[axis];

    if(usbNoToFileDesc[usbNumber]<0) return -1;
    int fd = usbNoToFileDesc[usbNumber];

    return fd;
}

int LYongMotor::CloseAll()
{
    int i;
    for(int i=0;i<16;i++)
    {
        Close(i);
    }

    return 0;
}
int LYongMotor::Close(int axis)
{
    return 0;
}

int LYongMotor::OpenAll()
{
    int i;
    for(int i=0;i<16;i++)
    {
        Open(i);
    }
    start();
    return 0;
}


int LYongMotor::Open(int usbNumber)
{
    char location[128];
    sprintf(location, "/dev/ttyUSB%d", usbNumber);

    int fd_1;
    fd_1 = open(location, O_NONBLOCK | O_RDWR | O_NOCTTY );

    if(fd_1>1)
    {
        usbNoToFileDesc[usbNumber] = fd_1;
        printf("\n Connect! \n");
    }
    else
    {
        close(fd_1);
        return -1;
    }

    set_interface_attribs (fd_1, B19200, 0);

    // get id from fd_1

    usleep(10*1000);
    YongIDData(usbNumber);


    // Axis   Yong Motor ID     Real Axis
    // 0      100               X
    // 1      101               Y
    // 2      102               Z
    /*
    if(idid == 100) axisToUsbNumber[0] = usbNumber;
    else if(idid == 101) axisToUsbNumber[1] = usbNumber;
    else if(idid == 102) axisToUsbNumber[2] = usbNumber;
    else
    {
        // unhandled motor
        printf("Un handled motor\n", idid);
    }
    */
}


int LYongMotor::YongIDData(int usbNumber)
{
    for(int i=102;i>=100;i--)
    {
        char setMotorID[] =
        {
            183
            ,184
            ,(char)i
            ,4
            ,1
            ,133
            ,0x00 // checksum
        };
        WriteMotorDataByUsb(usbNumber, setMotorID, sizeof(setMotorID));
        usleep(20 * 1000);
    }
    return 0;
}

int LYongMotor::YongReadData(int usbNumber)
{
    char buf[256];
    int buf_length = 0;
    int myfd = GetFileDescriptor(usbNumber);
    int monitor = read(myfd,buf,sizeof(buf));
    buf_length += monitor;
    for(int i=0; i<buf_length; i++)
    {
        printf("data[%d] is %d. \n",i,buf[i]);
    }

    return 0;
}

float LYongMotor::GetPosition(int axis)
{
    int motor_axis = axis + 100;
    char getPositionData[] =
    {
        183
        ,184
        ,(char)motor_axis
        ,4
        ,1
        ,193
        ,0x00 // checksum
    };
    WriteMotorData(axis, getPositionData, sizeof(getPositionData));

    return 0;

}

int LYongMotor::gpioExport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioExport unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        perror("gpioExport");
        return fileDescriptor ;

    }
    close(fileDescriptor);

    return 0;
}


int LYongMotor::gpioSetDirection ( jetsonGPIO gpio, unsigned int out_flag )
{
    int fileDescriptor;

    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);

    close(fileDescriptor);
    return 0;
}

int LYongMotor::gpioGetValue ( jetsonGPIO gpio, unsigned int *value)
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch;

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetValue unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, 1) != 1) {
        perror("gpioGetValue") ;
        return fileDescriptor ;
     }

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    close(fileDescriptor);
    return 0;
}

int LYongMotor::gpioUnexport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioUnexport unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        perror("gpioUnexport") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

int LYongMotor::gpio_set_value(void)
{
    jetsonGPIO pushButton = gpio160 ;
    gpioExport(pushButton) ;
    gpioSetDirection(pushButton,inputPin) ;
    unsigned int value = low;
    int countvalue = 0;

    printf("Start Input Check\n");
    gpioGetValue(pushButton, &value) ;
    if (value==high)
    {
        countvalue = 5;
    }
    else
    {
    countvalue = 0;
    printf("Fail\n");
    }


    usleep(1000); // sleep for a millisecond
    printf("Counting is %d \n",countvalue);
    printf("END");
    gpioUnexport(pushButton);
    return 0;
}

int LYongMotor::MotorCalc(int axis, float velocity, float distance)
{
    float max_distance=0;
    float calc_distance=0;
    float percent_velocity=0;
    float motor_rotation=0;
    //char pulse[3] = {MOTOR_X_CYCLE_PULSE,MOTOR_Y_CYCLE_PULSE,MOTOR_Z_CYCLE_PULSE};

    char rpm_max[] = {
        MOTOR_X_RPM_MAX
        ,MOTOR_Y_RPM_MAX
        ,MOTOR_Z_RPM_MAX
    };
    char ideal_distance[] = {
        MOTOR_X_CYCLE_DISTANCE
        ,MOTOR_Y_CYCLE_DISTANCE
        ,MOTOR_Z_CYCLE_DISTANCE
    };

    motor_rotation = distance/12;
    //one rotation clk = 12

    max_distance = rpm_max[axis]*ideal_distance[axis]/60;// mm/s
    //Maximum distance

    calc_distance = velocity*ideal_distance[axis]/60;// mm/s
    //calculate ideal distance
    //0=<calc_distance=<max_distance

    percent_velocity = velocity/rpm_max[axis]*100;


    return 0;
}

int LYongMotor::MotorConfigure(int axis,int accuracy)
{
    int motor_axis = axis + 100;
    char motor_accuracy[] = {
        183
        ,184
        ,(char)motor_axis
        ,167
        ,2
        ,accuracy
        ,0
        ,0 // checksum.
    };
    return 0;
}
