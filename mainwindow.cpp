//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));


    ui->setupUi(this);

    QObject::connect(&serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);

    ui->sendButton->setEnabled(false);

    ui->baudrateBox->setCurrentIndex(0);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
    //从界面中读取以前收到的数据
    QString recv = ui->recvTextEdit->toPlainText();
    recv += QString(buffer);
    //清空以前的显示
    ui->recvTextEdit->clear();
    //重新显示
    ui->recvTextEdit->append(recv);
}

void MainWindow::on_searchButton_clicked()
{
    ui->portNameBox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portNameBox->addItem(info.portName());
    }
}
void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==QString("打开串口"))
    {
        //设置串口名
        serial.setPortName(ui->portNameBox->currentText());
        //设置波特率
        serial.setBaudRate(ui->baudrateBox->currentText().toInt());
        //设置数据位数
        switch(ui->dataBitsBox->currentIndex())
        {
        case 8: serial.setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial.setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->stopBitsBox->currentIndex())
        {
        case 1: serial.setStopBits(QSerialPort::OneStop); break;
        case 2: serial.setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial.setFlowControl(QSerialPort::NoFlowControl);
        //打开串口
        if(!serial.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL, "提示", "无法打开串口！");
           return;
        }
        //下拉菜单控件失能
        ui->portNameBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->dataBitsBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->stopBitsBox->setEnabled(false);
        ui->openButton->setText(QString("关闭串口"));
        //发送按键使能
        ui->sendButton->setEnabled(true);
    }
    else
    {
        //关闭串口
        serial.close();
        //下拉菜单控件使能
        ui->portNameBox->setEnabled(true);
        ui->baudrateBox->setEnabled(true);
        ui->dataBitsBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->stopBitsBox->setEnabled(true);
        ui->openButton->setText(QString("打开串口"));
        //发送按键失能
        ui->sendButton->setEnabled(false);
    }
}

void MainWindow::on_sendButton_clicked()
{
    int x = ui->xTextEdit->toPlainText().toInt();
    int y = ui->yTextEdit->toPlainText().toInt();
    int z = ui->zTextEdit->toPlainText().toInt();
    QByteArray data;
    data.resize(11);
    data[0] = 0xff;
    data[1] = 0x11;
    data[2] = 0x06;
    data[3] = (char)(x>>0x08);
    data[4] = (char)(x&0xff);
    data[5] = (char)(y>>0x08);
    data[6] = (char)(y&0xff);
    data[7] = (char)(z>>0x08);
    data[8] = (char)(z&0xff);
    data[9] = (0xff+0x11+0x06+data[3]+data[4]+data[5]+data[6]+data[7]+data[8])&0xff;
    data[10] = 0x0d;
    serial.write(data);
    qDebug() << "x:"<<(int)x<<"y:"<<(int)y<<"z:"<<(int)z;

}

void MainWindow::on_sendOnceRand_clicked()
{
    int x1 = qrand() % 1000;//x范围 -100 - +100
    int x2 = -qrand() % 1000;
    int x = x1+x2;
    int y = qrand() % 1000;//y范围 0 - +100
    int z = qrand() % 1000;//z范围 0 - +100
    QByteArray data;
    data.resize(11);
    data[0] = 0xff;
    data[1] = 0x11;
    data[2] = 0x06;
    data[3] = (char)(x>>0x08);
    data[4] = (char)(x&0xff);
    data[5] = (char)(y>>0x08);
    data[6] = (char)(y&0xff);
    data[7] = (char)(z>>0x08);
    data[8] = (char)(z&0xff);
    data[9] = (0xff+0x11+0x06+data[3]+data[4]+data[5]+data[6]+data[7]+data[8])&0xff;
    data[10] = 0x0d;
    serial.write(data);
    qDebug() << "x:"<<(int)x<<"y:"<<(int)y<<"z:"<<(int)z;

}

void MainWindow::on_sendContinue_clicked()
{
    signleTime = 1000/(ui->sendFrqBox->value());
       speed = ui->speedBox->value();
       speed_x = speed*0.9;//cm/s
       speed_y = speed*0.5;//cm/s
       if (timer->isActive() == false)
          {
            timer->start(signleTime);
          }
};

void MainWindow::sendData()
{

    QByteArray data;
    data.resize(11);
    data[0] = 0xff;
    data[1] = 0x11;
    data[2] = 0x06;
    data[3] = (char)((char)fx>>0x08);
    data[4] = (char)((char)fx&0xff);
    data[5] = (char)((char)fy>>0x08);
    data[6] = (char)((char)fy&0xff);
    data[7] = (char)(23>>0x08);
    data[8] = (char)(23&0xff);
    data[9] = (0xff+0x11+0x06+data[3]+data[4]+data[5]+data[6]+data[7]+data[8])&0xff;
    data[10] = 0x0d;

    serial.write(data);

    qDebug() << "x:"<<(int)(fx+(qrand() % 1))<<"y:"<<(int)(fy+qrand() % 1);

       fx+= (float)speed_x*signleTime/(1000);
       fy = fx*0.5 + 50;//   y=1/2x+50

       if ((fy>=100|fx>=100 | fy<0)&timer->isActive() == true)
              {
                  timer->stop();
                  fx=-100;
                  fy=0;
              }
};

void MainWindow::on_clearButton_clicked()
{
    ui->recvTextEdit->clear();
}
