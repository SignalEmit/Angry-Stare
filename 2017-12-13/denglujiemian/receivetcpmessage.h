#ifndef RECEIVETCPMESSAGE_H
#define RECEIVETCPMESSAGE_H

#include<QTcpSocket>
#include <QtNetwork>
#include"tcpclass.h"
#include<QList>
#include<QObject>
template<class T>
class MessageBase
{
private:
    bool is_receivecount;               //是否接受过数字
protected:
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    int count;
    QList<T> EverythingList;
private:
    void receiveTcpMessage()
    {
        QByteArray message;
        QDataStream in(tcpSocket);
        in.setVersion(QDataStream::Qt_5_8);
        if (blockSize == 0)
        {
            if (tcpSocket->bytesAvailable() <(int)sizeof(quint16)) return;
            in >> blockSize;
        }
        if (tcpSocket->bytesAvailable() < blockSize) return;
        if(tcpSocket->bytesAvailable() >=blockSize)
        {
            in >> message;
            count=message.toInt();
            qDebug()<<"这个数字是：：：：：：："<<count;
            is_receivecount=true;               //已经接收到数字
            blockSize = 0;
        }
    }
public:
    int GetCount(){return count;}
    MessageBase(QTcpSocket *tcpSocket_)
    {
            is_receivecount=false;
            blockSize=0;
            tcpSocket=tcpSocket_;
    }
    void Receivelist()
    {
        if(!is_receivecount)                    //如果没有接收过数字的话先接收数字
        {
            receiveTcpMessage();
        }
        for(int i=0;i<count;i++)
        {
            QDataStream in(tcpSocket);
            in.setVersion(QDataStream::Qt_5_8);
            if (blockSize == 0)
            {
                if (tcpSocket->bytesAvailable() <(int)sizeof(quint16)) return;
                in >> blockSize;
            }
            if (tcpSocket->bytesAvailable() < blockSize) return;
            if(tcpSocket->bytesAvailable()>=blockSize)
            {
                ReceiveListMessage(in);
            }
        }
    }
protected:
    virtual void ReceiveListMessage(QDataStream &in)=0;
};

class ReceiveOrderTcpMessage:public QObject,public MessageBase<PayMessage>
{
    Q_OBJECT
signals:
    void SendTheList(QList<PayMessage> message);
public:
    ReceiveOrderTcpMessage(QTcpSocket *tcpSocket_):MessageBase<PayMessage>(tcpSocket_){}
    void ReceiveListMessage(QDataStream &in);
};
class ReceiveFriendTcpMessage:public QObject,public MessageBase<Friend>
{
    Q_OBJECT
signals:
    void SendTheList(QList<Friend> message);
public:
    ReceiveFriendTcpMessage(QTcpSocket *tcpSocket_):MessageBase<Friend>(tcpSocket_){}
    void ReceiveListMessage(QDataStream &in);
    void ReceiveOneFriendMessage(Friend &newFriend);
};
class ReceiveSearchQStringTcpMessage:public QObject,public MessageBase<QString>
{
    Q_OBJECT
signals:
    void SendTheList(QList<QString> message);
public:
    ReceiveSearchQStringTcpMessage(QTcpSocket *tcpSocket_):MessageBase<QString>(tcpSocket_){}
    void ReceiveListMessage(QDataStream &in);
};
class ReceiveProductTcpMessage:public QObject,public MessageBase<Procuct>
{
    Q_OBJECT
signals:
    void SendTheList(QList<Procuct> message);
public:
    ReceiveProductTcpMessage(QTcpSocket *tcpSocket_):MessageBase<Procuct>(tcpSocket_){}
    void ReceiveListMessage(QDataStream &in);
};
#endif // RECEIVETCPMESSAGE_H
