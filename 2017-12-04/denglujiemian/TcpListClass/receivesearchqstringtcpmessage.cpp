#include "receivesearchqstringtcpmessage.h"

void ReceiveSearchQStringTcpMessage::ReceiveListMessage()
{
    QString ANewQString;
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
        in >>ANewQString;
        blockSize=0;
        EverythingList.push_back(ANewQString);
        if(EverythingList.size()==count)                    //濡傛灉鎵€鏈変笢瑗跨殑澶у皬绛変簬鏁板瓧
            emit SendTheList(EverythingList);
    }
}
