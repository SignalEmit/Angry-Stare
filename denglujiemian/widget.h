#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPixmap>
#include<QStackedWidget>
#include"interface.h"
#include"dialoglogin.h"
#include<QtNetWork>
#include<QByteArray>
#include <QDataStream>
#include<QTcpSocket>
#include<QFile>

#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>

#include"tcpclass.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget,public TcpClass
{
    Q_OBJECT

public:
    QString LogInLabel;         //应显示的标签
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void pushbutton_clicked();
private:
    /*********************************数据库*************************/
    QSqlDatabase db;
    bool ShowComboBox;                      //显示下拉框
    bool FirstShowAutoLogin;
private:
    void showEvent(QShowEvent *e);
    void LogInDialog(QString Label);
    Interface newwidget;
    Ui::Widget *ui;
    /*****************************************************/
    /*******************网络服务各种变量*********************/
    /*****************************************************/
private:
    quint16 blockSize;
    QTcpSocket *tcpSocket_Login;                        //登录套接字
    QTcpSocket *tcpSocket_RegisterLogIn;                //注册套接字
    QTcpSocket *tcpSocket_CheckAccountRegisterLogIn;    //注册套接字
    QTcpSocket *tcpSocket_ForgetPassword;               //忘记密码套接字

private slots:
    void displayError(QAbstractSocket::SocketError);
    void sendMessage();
    void receiveMessage();
    void on_Login_clicked();
    void on_Check_RegisterLogInBt_clicked();
    void on_Ok_RegisterLogInBt_clicked();
    void on_Ok_ForgetPassword_clicked();

    void on_Account_lineedit_currentTextChanged(const QString &arg1);



};

#endif // WIDGET_H
