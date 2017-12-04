#include "receiveproducttcpmessage.h"
QDataStream& operator>>(QDataStream &in, Procuct &data)
{
    in>>data.ProductName>>data.CommodityCode>>data.Cost>>data.Number>>data.Synopsis>>data.MassUnit>>data.PictureLocation>>data.Postage>>data.Promulgator>>data.ContactNumber;
    return in;
}
void ReceiveProductTcpMessage::ReceiveListMessage()
{
    Procuct ANewProduct;
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
        in >>ANewProduct;
        blockSize=0;
        EverythingList.push_back(ANewProduct);
        if(EverythingList.size()==count)                    //濡傛灉鎵€鏈変笢瑗跨殑澶у皬绛変簬鏁板瓧
            emit SendTheList(EverythingList);
    }
}
