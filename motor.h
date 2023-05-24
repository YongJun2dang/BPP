#ifndef MOTOR_H
#define MOTOR_H

#include <QWidget>
#include "handler.h"

namespace Ui {
class Motor;
}

class Motor : public QWidget
{
    Q_OBJECT

public:
    explicit Motor(QWidget *parent = 0);
    ~Motor();

private:
    Ui::Motor *ui;

protected:
    void Add(int index = 0);
};

#endif // MOTOR_H
