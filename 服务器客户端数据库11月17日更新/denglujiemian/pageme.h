#pragma once
#ifndef PAGEME_H
#define PAGEME_H

#include <QWidget>
#include<QCloseEvent>
#include<QShowEvent>
#include<QScrollArea>
#include"dialog.h"
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include<QLabel>
#include<QGridLayout>
#include<QMouseEvent>

#include<QThread>
#include<QtNetWork>
#include<QByteArray>
#include<QTcpSocket>

#include"chat.h"
#include<QList>
#include"tcpclass.h"
#include"picturethread.h"

namespace Ui {
class PageMe;
}
class PageMe : public QWidget,public TcpClass
{
    Q_OBJECT
signals:
    void OpenPage1(int pageNumber);
    void ChatPageReceiveHeadPicture(QString);                          //去Chat页面接收好友的头像
    void createChatSocket();                                           //创建聊天套接字
public:
    explicit PageMe(QWidget *parent = 0);
    ~PageMe();

private:
    QScrollArea *ScrollAreaPage1;
    QScrollArea *ScrollAreaPage3;                       //滑块区

private:
    Dialog dialog_close;
    Ui::PageMe *ui;
    /**********************88闪烁*********************/
private slots:
    void TimeStar();
    void TimeEnd();
    void SetFriendPicture(QString);             //完成好友列表图片的显示
private:
    QTimer timestar[50];
    QTimer timeend[50];

 /**********************88闪烁*********************/
private:

    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);

private:
    Chat man;
    int x_mousePress;                   //按下的x
    int y_mousePress;                   //按下的y
    int x_mouseRelease;                 //释放的x
    int y_mouseRelease;                 //释放的y
    /**************消息界面**************/
   // bool is_FristShowTheCount;

    QLabel **Label1_Text;
    QLabel **Label2_Text;
    QLabel **Label3_Text;
    QLabel **Label4_Text;
    QWidget **Widget_Text;
    QGridLayout **GridLayout_Text;

    /**************************************/

    QLabel *friendlabel[50];
    QLabel *friendpicturelabel[50];
    QHBoxLayout *friendHBoxLayout[50];

    /**************************************网络****************************/

private:

    bool showAgain;         //是否已经显示过
    QTcpSocket *tcpSocket_FriendList;                        //登录套接字

    QTcpSocket *tcpSocket_ANewChatMessageFriend;                //一个新的聊天好友套接字
    QList<Friend> FriendList;       //朋友列表
    QList<QString> ChatUserList;                        //消息界面链表


private:
    void ChatMessagePageAddNews(QString ChatMessageFriendAccount,QString ChatMessage,bool is_Shine);

private slots:
    void setChatPicture(QString);               //设置接收到的图片
    void activeChatSocket()
    {
        emit createChatSocket();
        Label1_Text=new QLabel*[50];
        Label2_Text=new QLabel*[50];
        Label3_Text=new QLabel*[50];
        Label4_Text=new QLabel*[50];
        Widget_Text=new QWidget*[50];
        GridLayout_Text=new QGridLayout*[50];
    }                    //建立聊天套接字
    void sendMessage();
    void receiveMessage();
    void displayError(QAbstractSocket::SocketError);

    void on_BackBt_clicked();
    void on_NewsBt_clicked();
    void on_AddFriend_clicked();
    void AddSuccessInList(Friend);          //添加好友后链表加1
    void ChatMessageShowNewUser(QString ChatMessageFriendAccount,QString ChatMessage,bool is_Shine);

};

#endif // PAGEME_H
