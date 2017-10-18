#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include<QCloseEvent>
#include<QTimer>
#include"tcpclass.h"
namespace Ui {
class DialogLogIn;
}

class DialogLogIn : public QDialog,public TcpClass
{
    Q_OBJECT
signals:
    void ShowInterFacePage();
public:
    explicit DialogLogIn(QWidget *parent = 0);
    ~DialogLogIn();

private slots:
    void on_pushButton_clicked();

    void TimeEnd();
private:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
    Ui::DialogLogIn *ui;
private:
    int TimeCount;          //时间计数器
    QTimer CountTime;
};

#endif // DIALOGLOGIN_H
