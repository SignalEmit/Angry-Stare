
#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include<QShowEvent>
#include<QTcpSocket>
#include"tcpclass.h"
#include<QDataStream>
#include<QThread>
#include<QListWidgetItem>
#include<QTextCodec>
#include"picturethread.h"
namespace Ui {
class Chat;
}

class Chat : public QWidget,public TcpClass
{
    Q_OBJECT
signals:
    void PushBackInFriendList(Friend aNewFriend);
    void receiveChatMessage(QString,QString,bool);
    void threadStart();

public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();

private slots:
    void setSearchFriendPicture(QString);
    void on_Back_clicked();

    void on_SendMessageBt_clicked();

    void on_SendMessage_clicked();
public:
    QString LogInLabel;
private:
    bool  is_ChatPageShow;                  //聊天界面是否显示
    Ui::Chat *ui;


    /****************************************88滑动区***************************************/
    QScrollBar *m_scrollBarV[2];                           //绑定滑动事件
    int press_y[2];
    int move_y[2];
    int release_y[2];
    QDateTime pressDateTime[2];
    QPropertyAnimation *animation[2];

private:
    void BuildFilePathWriteChatMessage(QString Judge,QString newMan="",QString newMessage="");
    void receiveNewFriend(QTcpSocket *tcpSocket,newPeople &message);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    /********************************网络**********************/
private:
    QTextCodec *TextCodec_UTF8;                 //文本转换UTF-8格式


    bool JudgeLoginUpFrist;                     //纪录是不是第一次进入
    QTcpSocket *ChatWithFriendTcpSocket;        //聊天套接字
    QTcpSocket *AddFriendTcpSocket;             //添加好友套接字并返回好友信息
    QTcpSocket *AddFriendSuccees;               //成功添加好友入数据库



    newPeople newFriend;            //接收新朋友的账号信息
private:
    QString setChatPicture(QString picturePath,QString pictureWidth,QString pictureHeigth,QString chatString);      //设置聊天图片和聊天记录
    friend QDataStream& operator>>(QDataStream &in, newPeople &data);

private slots:
    void sendMessage();
    void receiveMessage();
    void on_SearchFriendAccount_textChanged(const QString &arg1);
    void on_SearchFriendBt_clicked();


    void on_AddFriendBt_clicked();
};
#endif // CHAT_H
