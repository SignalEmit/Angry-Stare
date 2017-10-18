#include "tcpclass.h"
#include<QDebug>
#include<QFile>
#include<QDesktopWidget>
#include<QApplication>
#include<QDataStream>
TcpClass::TcpClass()
{

}

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

void TcpClass::receiveTcpMessage_ListFarmPruduct(QTcpSocket *tcpSocket,Procuct &_list)
{
    Procuct message;
    quint16 blockSize = 0;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_8);
    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() <(int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
    _list=message;
}
/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/
/********************************************88接收好友列表************************************************/

QDataStream& operator>>(QDataStream &in, Friend &data)
{
    in>>data.FriendCount>>data.Remark>>data.Age>>data.NickName>>data.Sex>>data.Area>>data.NoYesFriend;
    return in;
}
void TcpClass::receiveTcpMessage_FriendList_Search(QTcpSocket *tcpSocket,Friend &message)
{
    quint16 blockSize = 0;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_8);
    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() <(int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
}


/*********************************发送农产品套接字***********************************/
QDataStream& operator<<(QDataStream &out, Procuct &data)
{
    out<<data.ProductName<<data.CommodityCode<<data.Cost<<data.Number<<data.Synopsis<<data.MassUnit<<data.PictureLocation<<data.Postage<<data.Promulgator<<data.ContactNumber;
    return out;
}
void TcpClass::SendFarmProduct(Procuct message,QTcpSocket *tcpSocket_1)
{
    QByteArray ByteArray; //用于暂存我们要发送的数据
    QDataStream out(&ByteArray, QIODevice::WriteOnly);
    out<< (quint16)0;
    out<< message;
    out.device()->seek(0);
    out<< (quint16)(ByteArray.size() - sizeof(quint16));
    tcpSocket_1->write(ByteArray);
}


void TcpClass::newConnect(QTcpSocket *tcpSocket)
{
    tcpSocket->abort();                             //取消已有的连接
    tcpSocket->connectToHost(QHostAddress("192.168.137.1"), 666);
}
void TcpClass::SendChatMessage(QTcpSocket *tcpSocket)
{
    tcpSocket->connectToHost(QHostAddress("192.168.137.1"), 666);
}

void TcpClass::SendTcpMessage(QString message,QTcpSocket *tcpSocket_1)
{
    QByteArray ByteArray; //用于暂存我们要发送的数据
    QDataStream out(&ByteArray, QIODevice::WriteOnly);
    out<< (quint16)0;
    out<< message.toUtf8();
    qDebug() << message;
    out.device()->seek(0);
    out<< (quint16)(ByteArray.size() - sizeof(quint16));
    tcpSocket_1->write(ByteArray);
}
void TcpClass::receiveTcpMessage(QString &Judge,QTcpSocket *tcpSocket)
{
    QByteArray message;
    blockSize = 0;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_8);
    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() <(int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
    Judge=message;
}
bool TcpClass::SlidingRegin(QEvent *event, QObject *obj, QScrollBar *m_scrollBarV,QPropertyAnimation *animation, QDateTime &pressDateTime, int &press_y,int &move_y,int &release_y)
{

         int scrollV_max = m_scrollBarV->maximum();
         int scrollV_min = m_scrollBarV->minimum();

         //根据鼠标的动作——按下、放开、拖动，执行相应的操作
         if (event->type() == QEvent::MouseButtonPress)
         {
             //记录按下的时间、坐标

             pressDateTime = QDateTime::currentDateTime();
             move_y = QCursor::pos().y();
             press_y = move_y;

             animation->stop();
         }
         else if (event->type() == QEvent::MouseButtonRelease)
         {
             //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离

             if (animation->targetObject() != m_scrollBarV)
             {
                 animation->setTargetObject(m_scrollBarV);
                 animation->setPropertyName("value");
             }

             move_y = -1;
             release_y = QCursor::pos().y();
             //        qDebug()<<"MouseButtonRelease QCursor::pos().y()="<<QCursor::pos().y();
             QObject *parent_obj = obj->parent();
             if (parent_obj != 0 || parent_obj->inherits("QAbstractItemView"))
             {
                 ;
                 // QTimer::singleShot(150, (QAbstractItemView *)parent_obj , SLOT(clearSelection()));
             }

             int endValue;
             int pageStep;
             if (release_y - press_y != 0 && qAbs(release_y - press_y) > 45)
             {
                 //qDebug()<<"obj->objectName()="<<obj->objectName();
                 int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
                 //            qDebug()<<"mseconds="<<mseconds;

                 int limit = 440;
                 pageStep = 240;//scrollBarV->pageStep();
                                //            qDebug()<<"pageStep="<<pageStep;
                 if (mseconds > limit)//滑动的时间大于某个值的时候，不再滚动(通过增加分母)
                 {
                     mseconds = mseconds + (mseconds - limit) * 20;
                 }

                 if (release_y - press_y > 0)
                 {
                     endValue = m_scrollBarV->value() - pageStep * (200.0 / mseconds);//.0避免避免强制转换为整形
                     if (scrollV_min > endValue)
                     {
                         endValue = scrollV_min;
                     }
                 }
                 else if (release_y - press_y < 0)
                 {
                     endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
                     if (endValue > scrollV_max)
                     {
                         endValue = scrollV_max;
                     }
                 }
                 if (mseconds > limit)
                 {
                     mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
                 }
                 animation->setDuration(mseconds + 550);
                 animation->setEndValue(endValue);
                 animation->setEasingCurve(QEasingCurve::OutQuad);
                 animation->start();
                 return true;
             }
         }
         else if (event->type() == QEvent::MouseMove && move_y >= 0)
         {
             //窗口跟着鼠标移动

             int move_distance = QCursor::pos().y() - move_y;
             int endValue = m_scrollBarV->value() - move_distance;
             if (scrollV_min > endValue)
             {
                 endValue = scrollV_min;
             }

             if (endValue > scrollV_max)
             {
                 endValue = scrollV_max;
             }
             m_scrollBarV->setValue(endValue);
             //qDebug()<<"endValue="<<endValue;
             //qDebug()<<"move_distance="<<move_distance;
             move_y = QCursor::pos().y();
         }
         return false;
}

int TcpClass::heigth_Change(float ss)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    qDebug()<<"此界面高是："<<screenRect.height();
    int sWidth = screenRect.height()*ss;
    return sWidth;
}

int TcpClass::width_Change(float ss)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    qDebug()<<"此界面宽是："<<screenRect.width();
    int sWidth = screenRect.width()*ss;
    return sWidth;
}

