#ifndef TCPCLASS_H
#define TCPCLASS_H
#include<QtNetWork>
#include<QByteArray>
#include <QDataStream>
#include<QTcpSocket>
#include<QScrollBar>
#include<QPropertyAnimation>

class Procuct                                           //农产品
{
public:
    QString ProductName;            //商品名称
    QString CommodityCode;          //商品编号
    float Cost;                   //商品价格
    int Number;                 //数量
    QString Synopsis;               //简介
    QString MassUnit;           //质量单位
    QString PictureLocation;      //图片位置
    float Postage;                    //邮费
    QString Promulgator;            //发送者账号
    QString ContactNumber;          //发布者电话
    Procuct()
    {

    }

    Procuct( QString CommodityCode_s,QString procuct_s,float Cost_s,int Number_s,QString MassUnit_s,QString Synopsis_s,QString PictureLocation_s,float Postage_s,QString Promulgator_s,QString ContactNumber_s)
    {
        CommodityCode=CommodityCode_s;
        ProductName=procuct_s;
        Cost=Cost_s;
        Number=Number_s;
        Synopsis=Synopsis_s;
        MassUnit=MassUnit_s;
        PictureLocation=PictureLocation_s;
        Postage=Postage_s;
        Promulgator=Promulgator_s;
        ContactNumber=ContactNumber_s;
    }
};

class newPeople
{
public:
    QString FriendCount;        //朋友账号
    QString Age;            //年龄
    QString NickName;           //昵称
    QString Sex;
    QString Area;
    bool NoYesFriend;
public:
    newPeople(){}
    newPeople(QString FriendCount_,QString Age_,QString NickName_,QString Sex_,QString Area_,bool NoYesFriend_)
    {
        FriendCount=FriendCount_;
        Age=Age_;
        NickName=NickName_;
        Sex=Sex_;
        Area=Area_;
        NoYesFriend=NoYesFriend_;
    }
};


class Friend
{
public:
    QString FriendCount;        //朋友账号
    QString Remark;             //备注
    QString Age;            //年龄
    QString NickName;           //昵称
    QString Sex;
    QString Area;
    bool NoYesFriend;
public:
    Friend(){}
    Friend(QString FriendCount_,QString Remark_,QString Age_,QString NickName_,QString Sex_,QString Area_,bool NoYesFriend_)
    {
        FriendCount=FriendCount_;
        Remark=Remark_;
        Age=Age_;
        NickName=NickName_;
        Sex=Sex_;
        Area=Area_;
        NoYesFriend=NoYesFriend_;
    }
};


class TcpClass
{
public:
    TcpClass();
public:
    enum PicTureType{HeadPicture,FarmProduct};
public:

    bool SlidingRegin(QEvent *event, QObject *obj, QScrollBar *m_scrollBarV,QPropertyAnimation *animation, QDateTime &pressDateTime, int &press_y,int &move_y,int &release_y);
    void newConnect(QTcpSocket *tcpSocket);
    void SendChatMessage(QTcpSocket *tcpSocket);
    void SendTcpMessage(QString message,QTcpSocket *tcpSocket_1);
    void receiveTcpMessage(QString &Judge,QTcpSocket *tcpSocket);
    int heigth_Change(float ss);
    int width_Change(float ss);

    void receiveTcpMessage_ListFarmPruduct(QTcpSocket *tcpSocket,Procuct &s);
    void receiveTcpMessage_FriendList_Search(QTcpSocket *tcpSocket,Friend &message);

    void SendFarmProduct(Procuct message,QTcpSocket *tcpSocket_1);              //发送产品套接字
private:
    quint16 blockSize;


};

#endif // TCPCLASS_H
