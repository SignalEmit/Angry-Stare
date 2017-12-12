#include "receivetcpmessage.h"
#include<QDataStream>
/********************************************88接收农产品************************************************/
/********************************************88接收农产品************************************************/
/********************************************88接收农产品************************************************/

#ifndef QT_NO_DATASTREAM
QDataStream& operator>>(QDataStream &in, Procuct &data)
{
    in>>data.ProductName>>data.CommodityCode>>data.Cost>>data.Number>>data.Synopsis>>data.MassUnit>>data.PictureLocation>>data.Postage>>data.Promulgator>>data.ContactNumber;
    return in;
}
#endif

/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/

QDataStream& operator>>(QDataStream &in, Friend &data)
{
    in>>data.FriendCount>>data.Remark>>data.Age>>data.NickName>>data.Sex>>data.Area>>data.NoYesFriend>>data.FriendPictureSize;
    return in;
}
/*********************************************接受以购买过的商品******************************/
/*********************************************接受以购买过的商品******************************/
/*********************************************接受以购买过的商品******************************/
#ifndef QT_NO_DATASTREAM
QDataStream& operator>>(QDataStream &in, PayMessage &data)
{
    in>>data.OrderCommodityCode>>data.PayGoodsMassUnit>>data.PayGoodsNumber>>data.PayGoodsMoney>>data.CommodityCode>>data.ProductName>>data.Cost>>data.Number>>data.Synopsis>>data.MassUnit>>data.PictureLocation>>data.Postage>>data.Promulgator>>data.ContactNumber;
    return in;
}
#endif

void ReceiveFriendTcpMessage::ReceiveListMessage(QDataStream &in)
{
    Friend ANewFriend;
    in >>ANewFriend;
    blockSize=0;
    EverythingList.push_back(ANewFriend);
    if(EverythingList.size()==count)                    //如果所有东西的大小等于数字
        emit SendTheList(EverythingList);

}
void ReceiveFriendTcpMessage::ReceiveOneFriendMessage(Friend &newFriend)
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
        in>>newFriend;
    }
}

void ReceiveProductTcpMessage::ReceiveListMessage(QDataStream &in)
{
    Procuct ANewProduct;
    in >>ANewProduct;
    blockSize=0;
    EverythingList.push_back(ANewProduct);
    if(EverythingList.size()==count)                    //如果所有东西的大小等于数字
        emit SendTheList(EverythingList);
}
void ReceiveSearchQStringTcpMessage::ReceiveListMessage(QDataStream &in)
{ 
    QByteArray message;
    in >>message;
    blockSize=0;
    QString newString=message;
    EverythingList.push_back(newString);
    if(EverythingList.size()==count)                    //如果所有东西的大小等于数字
        emit SendTheList(EverythingList);
}
void ReceiveOrderTcpMessage::ReceiveListMessage(QDataStream &in)
{
    PayMessage newMessage;
    in >>newMessage;
    qDebug()<<"接了个啥："<<newMessage.CommodityCode;
    blockSize=0;
    EverythingList.push_back(newMessage);
    if(EverythingList.size()==count)                    //如果所有东西的大小等于数字
        emit SendTheList(EverythingList);
}
