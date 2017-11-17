#ifndef MYSETTINGPAGE_H
#define MYSETTINGPAGE_H

#include <QWidget>
#include<tcpclass.h>
#include<QThread>
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
public:
    explicit MySettingPage(QWidget *parent = 0);
    ~MySettingPage();
private:
    Ui::MySettingPage *ui;
private slots:
    void SendPicture(QString);
    void on_HeadSettingBt_clicked();
    void SetPage(int pageNumber);


    void on_CloseBt_clicked();
};

#endif // MYSETTINGPAGE_H
