#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H
#include<QTcpSocket>
#include <QtNetwork>
#include"tcpclass.h"
#include<QList>
#include<QObject>
template<class T>
class MessageBase
{
public:
    quint16 blockSize;
    QTcpSocket *tcpSocket;
    int count;
    bool is_receivecount;
    void receiveTcpMessage();
    QList<T> EverythingList;
public:
    int GetCount(){return count;}
    MessageBase(QTcpSocket *tcpSocket_);
};
#endif // MESSAGEBASE_H
