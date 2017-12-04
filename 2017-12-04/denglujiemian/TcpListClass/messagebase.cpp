#include "messagebase.h"
#include<QDataStream>
template<class T>
MessageBase<T>::MessageBase(QTcpSocket *tcpSocket_)
{
    is_receivecount=false;
    blockSize=0;
    tcpSocket=tcpSocket_;
}
template<class T>
void MessageBase<T>::receiveTcpMessage()
{
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
        in >> count;
        is_receivecount=true;               //宸茬粡鎺ユ敹鍒版暟瀛?
        blockSize = 0;

    }
}

