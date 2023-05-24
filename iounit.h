#ifndef IOUNIT_H
#define IOUNIT_H

#include <QWidget>

namespace Ui {
class IOUnit;
}

class IOUnit : public QWidget
{
    Q_OBJECT

public:
    explicit IOUnit(QWidget *parent = 0);
    ~IOUnit();
    void SetIndex(int index);
    int GetIndex();
    void ChangeInputColor(int flag);

private slots:
    void on_pushButtonOutput_clicked();

private:
    Ui::IOUnit *ui;
    int index;
};

#endif // IOUNIT_H
