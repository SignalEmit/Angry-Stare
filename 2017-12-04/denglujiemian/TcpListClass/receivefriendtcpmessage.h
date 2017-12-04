#ifndef RECEIVEFRIENDTCPMESSAGE_H
#define RECEIVEFRIENDTCPMESSAGE_H


#include<QTcpSocket>
#include <QtNetwork>
#include"tcpclass.h"
#include<QList>
#include<QObject>
#include"messagebase.h"
class ReceiveFriendTcpMessage:public QObject,public MessageBase<Friend>
{
    Q_OBJECT
signals:
    void SendTheList(QList<Friend> message);
public:
    ReceiveFriendTcpMessage(QTcpSocket *tcpSocket_):MessageBase<Friend>(tcpSocket_){}
    void ReceiveListMessage();
    void Receivelist()
    {
        if(!is_receivecount)                    //濡傛灉娌℃湁鎺ユ敹杩囨暟瀛楃殑璇濆厛鎺ユ敹鏁板瓧
            receiveTcpMessage();
        else                                    //鎺ユ敹杩囩殑璇?
        {
            for(int i=0;i<count;i++)
                ReceiveListMessage();
        }
    }
};

#endif // RECEIVEFRIENDTCPMESSAGE_H
