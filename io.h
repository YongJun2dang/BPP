#ifndef IO_H
#define IO_H

#include <QWidget>

namespace Ui {
class IO;
}

class IO : public QWidget
{
    Q_OBJECT

public:
    explicit IO(QWidget *parent = 0);
    ~IO();

private:
    Ui::IO *ui;

protected:
    void Add(int index = 0);
};

#endif // IO_H
