#ifndef LYONGMOTOR_H
#define LYONGMOTOR_H
#include "lmotor.h"
#include <QThread>
#include <queue>
#include "handler.h"

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64
typedef unsigned int jetsonGPIO ;
typedef unsigned int pinDirection ;
typedef unsigned int pinValue ;



template<class T, class C=std::deque<T> >
struct pubqueue : std::queue<T, C> {
  using std::queue<T, C>::c;

  static C& get_c(std::queue<T, C> &s) {
    return s.*&pubqueue::c;
  }
  static C const& get_c(std::queue<T, C> const &s) {
    return s.*&pubqueue::c;
  }
};

template<class T, class C>
C& get_c(std::queue<T, C> &a) {
  return pubqueue<T, C>::get_c(a);
}
template<class T, class C>
C& get_c(std::queue<T, C> const &a) {
  return pubqueue<T, C>::get_c(a);
}


class LYongMotor : public QThread, public LMotor
{
    Q_OBJECT

public:
    int gpioUnexport ( jetsonGPIO gpio );
    int gpio_set_value(void);
    int gpioExport ( jetsonGPIO gpio );
    int gpioGetValue ( jetsonGPIO gpio, unsigned int *value);
    int gpioSetDirection ( jetsonGPIO gpio, unsigned int out_flag );
    enum pinValues {
        low = 0,
        high = 1,
        off = 0,  // synonym for things like lights
        on = 1
    }  ;
    enum pinDirections {
        inputPin  = 0,
        outputPin = 1
    } ;

    enum jetsonGPIONumber {
        gpio57  =  57,    // J3A1 - Pin 50
        gpio160 = 160,	  // J3A2 - Pin 40
        gpio161 = 161,    // J3A2 - Pin 43
        gpio162 = 162,    // J3A2 - Pin 46
        gpio163 = 163,    // J3A2 - Pin 49
        gpio164 = 164,    // J3A2 - Pin 52
        gpio165 = 165,    // J3A2 - Pin 55
        gpio166 = 166     // J3A2 - Pin 58
    }  ;

public:
    LYongMotor();
    static const int MOTOR_X = 0x0;
    static const int MOTOR_Y = 0x1;
    static const int MOTOR_Z = 0x2;

    virtual int MoveInPulse(int axis, float velocity, float position);
    virtual int MoveRInPulse(int axis, float velocity, float distance) ;
    virtual int ReverseMoveInPulse(int axis, float velocity, float distance);
    virtual int Stop(int axis);
    virtual int MoveCont(int axis, float velocity);
    virtual float GetPosition(int axis);
    virtual int IsMoving(int axis);
    virtual int ReverseMoveCont(int axis, float velocity);
    virtual int ReverseMoveRInPulse(int axis, float velocity, float distance);
    virtual int OpenAll();
    virtual int Open(int axis);
    virtual int CloseAll();
    virtual int Close(int axis);

protected:
    int axisToUsbNumber[3]; // axis -> usb number
    int GetFileDescriptor(int axis);

public:
    int YongIDData(int usbNumber);
    int YongReadData(int usbNumber);
    int MotorConfigure(int axis,int accuracy);
    int MotorCalc(int axis, float velocity, float distance);


protected:

    int WriteMotorDataByUsb(int usbno, char* command, int len);
    int WriteMotorData(int axis, char* command, int length);
    int UpdateFromSerial(int usbNo);
    int ParsingSerial(int usbNo);

    static const int LengthUsbNo = 16;
    int usbNoToFileDesc[16]; // usb number -> file desc
    // myQueue[usbNo] -> queue
    pubqueue<char> myQueue[16];
private:
    void run();

};

#endif // LYONGMOTOR_H
