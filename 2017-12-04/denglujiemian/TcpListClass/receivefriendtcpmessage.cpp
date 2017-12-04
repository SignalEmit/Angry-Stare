#include "receivefriendtcpmessage.h"
QDataStream& operator>>(QDataStream &in, Friend &data)
{
    in>>data.FriendCount>>data.Remark>>data.Age>>data.NickName>>data.Sex>>data.Area>>data.NoYesFriend>>data.FriendPictureSize;
    return in;
}
void ReceiveFriendTcpMessage::ReceiveListMessage()
{
    Friend ANewFriend;
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
        in >>ANewFriend;
        blockSize=0;
        EverythingList.push_back(ANewFriend);
        if(EverythingList.size()==count)                    //濡傛灉鎵€鏈変笢瑗跨殑澶у皬绛変簬鏁板瓧
            emit SendTheList(EverythingList);
    }
}
