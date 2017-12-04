#ifndef PICTURETHREAD_H
#define PICTURETHREAD_H


#include<QTcpSocket>
#include<QByteArray>
#include<QFile>
#include<QDir>
#include<QTimer>
#include"tcpclass.h"
class PictureThread : public QObject,public TcpClass
{
    Q_OBJECT
signals:
    void SendFileName(QString);
public:
    PictureThread();
    PictureThread(int Number_,QString FilePath_,QString PictureName_);              //接图片
    PictureThread(int Number_,int PictrueType_,QString FilePath_,QString ServerFileName_);                                    //发图片
    ~PictureThread();

private:
    int PictrueType;
    QString fileName;
    int Number;
    QString PictureName;
    QString FilePath;
    QString ServerFileName;                     //服务器上文件名

    qint64 bytesReceived;  //已收到数据的大小
    qint64 totalBytes;
    qint64 fileNameSize;
    QByteArray inBlock;   //数据缓冲区

    QFile *localFile;  //要发送的文件


    QTcpSocket *tcpSocketPicture;      //推荐的图片
    QTcpSocket *SendPictureToServer;      //推荐的图片


  /*************************发送图片************************************/
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    void startTransfer();                               //图片信息转换

private slots:
    void sendFile(qint64 numBytes);                     //发送图片事件

    void sendMessage();                                 //服务器接收图片信号
    void sendPictureToServerMessage();                  //服务器发送图片信号
    void receiveFile();                                 //接收图片事件
private:
    void GOGOGO();                                      //接收图片触发器
    void SendPictureSlot();                             //发送图片触发器
};

#endif // PICTURETHREAD_H
