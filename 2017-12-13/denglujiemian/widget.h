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
#include<QList>

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
    QList<Interface*> theInterFaceList;
    QSqlDatabase db;
    bool is_click_loginbt;              //是否点击登录按键
    //QString loginAccount;               //登录账号
    //QString loginPassward;              //登录密码
private:
    void showEvent(QShowEvent *e);
    void LogInDialog(QString Label);
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
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void displayError(QAbstractSocket::SocketError);
    void sendMessage();
    void receiveMessage();
    void on_Login_clicked();
    void on_Check_RegisterLogInBt_clicked();
    void on_Ok_RegisterLogInBt_clicked();
    void on_Ok_ForgetPassword_clicked();
};

#endif // WIDGET_H
