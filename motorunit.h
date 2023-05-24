#ifndef MOTORUNIT_H
#define MOTORUNIT_H

#include <QWidget>
#include "handler.h"

namespace Ui {
class MotorUnit;
}

class MotorUnit : public QWidget
{
    Q_OBJECT

public:
    explicit MotorUnit(QWidget *parent = 0);
    ~MotorUnit();
    void SetIndex(int index);
    int GetIndex();

private slots:
    void on_pushButtonOn_clicked();

    void on_pushButtonPosSlow_pressed();

    void on_pushButtonPosSlow_released();

    void on_pushButtonPosFast_pressed();

    void on_pushButtonPosFast_released();

    void on_pushButtonNegSlow_pressed();

    void on_pushButtonNegSlow_released();

    void on_pushButtonNegFast_pressed();

    void on_pushButtonNegFast_released();


    void on_pushButtonHMinus_clicked();

    void on_pushButtonMove_clicked();

    void on_pushButtonReset_clicked();

private:
    Ui::MotorUnit *ui;
    int index;
    int posSlowPressed=0;
};

#endif // MOTORUNIT_H
