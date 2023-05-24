#ifndef TRACKINGLABEL_H
#define TRACKINGLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPaintEvent>

class TrackingLabel : public QLabel
{
    Q_OBJECT
public:
    TrackingLabel(QWidget *parent);
    void mousePressEvent(QMouseEvent* ev);
    void trackPosition(QString x="0", QString y="0");
    void paintEvent(QPaintEvent * e);
    int paintFlag=1;
    int positionX;
    int positionY;
};

#endif // TRACKINGLABEL_H
