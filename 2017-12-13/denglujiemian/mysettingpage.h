#ifndef MYSETTINGPAGE_H
#define MYSETTINGPAGE_H

#include <QWidget>
#include<tcpclass.h>
#include<QThread>
#include<QTcpSocket>
#include<picturethread.h>
#include"choosefile.h"
namespace Ui {
class MySettingPage;
}

class MySettingPage : public QWidget,public TcpClass
{
    Q_OBJECT
signals:
    void SetHeadPicture(QString);                   //设置头像
    void MyMessageChange(Friend);
public:
    explicit MySettingPage(QWidget *parent = 0);
    ~MySettingPage();
private:
    Ui::MySettingPage *ui;
private:
    QTcpSocket *personMessage_TcpSocket;
private:
    void closeEvent(QCloseEvent *e);
private slots:
    void sendMessage();
    void receiveMessage();
    void SendPicture(QString);
    void on_HeadSettingBt_clicked();
    void SetPage(int pageNumber,Friend myMessage);


    void on_CloseBt_clicked();
    void on_MyMesaageBt_clicked();
    void on_ChangePersonMessageBt_clicked();
};

#endif // MYSETTINGPAGE_H
