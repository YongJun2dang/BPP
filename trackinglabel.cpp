#include "trackinglabel.h"
#include <QMessageBox>
#include <QPainter>
#include <QPen>

TrackingLabel::TrackingLabel(QWidget *parent) :
QLabel(parent)
{

}

void TrackingLabel::trackPosition(QString x, QString y)
{
    QMessageBox::information(this, "좌표", "X="+x+",Y="+y);

}

void TrackingLabel::mousePressEvent(QMouseEvent *ev)
{
    QString x =  QString::number(ev->x());
    QString y =  QString::number(ev->y());

    positionX = ev->x();
    positionY = ev->y();
    paintFlag = 1;
    update();

    trackPosition(x, y);
}


void TrackingLabel :: paintEvent(QPaintEvent * e)
{
    QLabel::paintEvent(e);

    if(paintFlag)
    {
      QPainter painter(this);
      QPen paintpen(Qt::green);
      paintpen.setWidth(1);
      QPoint p1;
      p1.setX(positionX-6);
      p1.setY(positionY+2);
      painter.setPen(paintpen);
      //painter.drawPoint(p1);
      painter.drawText(p1,"╂");
   }
}
