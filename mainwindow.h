#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *timer;

private slots:
    void on_buttonStart_clicked();

    void on_buttonOnce_clicked();

    void on_buttonStop_clicked();

    void on_buttonConfig_clicked();

    void on_buttonExit_clicked();

    void on_pushButtonMotors_clicked();

    void on_pushButtonIOs_clicked();

    void on_checkBoxProcessImage_stateChanged(int arg1);


public slots:
     void changeLabel(QImage);

     void changeLabelRight(QImage);



     void onTimer();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
