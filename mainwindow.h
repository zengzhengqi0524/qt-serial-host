#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include<QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
explicit MainWindow(QWidget *parent = 0);
~MainWindow();
private slots:
void serialPort_readyRead();
void on_searchButton_clicked();
void on_openButton_clicked();
void on_sendButton_clicked();
void on_clearButton_clicked();
void on_sendOnceRand_clicked();
void on_sendContinue_clicked();
void sendData();
private:
Ui::MainWindow *ui;
QSerialPort serial;
QTime time;
QTimer *timer;

float fx=-100;
float fy=0;
char speed;
char speed_x;
char speed_y;
int  signleTime;
};
#endif // MAINWINDOW_H
