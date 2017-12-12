#include "picturethread.h"
#include<QDataStream>
PictureThread::PictureThread()
{

}

PictureThread::PictureThread(int Number_,int PictrueType_,QString FilePath_,QString ServerFileName_)   //发图片     //第一个参数传给服务器的编号，第二个图片类型，第三个文件路径，第四个需要修改的文件的编号
{
    ServerFileName=ServerFileName_;
    PictrueType=PictrueType_;                       //照片类型
    Number=Number_;
    FilePath=FilePath_;
    localFile=new QFile;


    loadSize = 4*1024;
    bytesWritten = 0;
    bytesToWrite = 0;

    //qDebug()<<"图片地址"<<FilePath;
    SendPictureToServer=new QTcpSocket(this);
    connect(SendPictureToServer, SIGNAL(connected()), this, SLOT(sendPictureToServerMessage()));
    SendPictureSlot();

}

PictureThread::PictureThread(int Number_,QString FilePath_,QString PictureName_)
{
    Number=Number_;
    PictureName=PictureName_;
    FilePath=FilePath_;

    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    localFile=new QFile;
    tcpSocketPicture=new QTcpSocket(this);
    connect(tcpSocketPicture, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocketPicture, SIGNAL(readyRead()), this, SLOT(receiveFile()));
    GOGOGO();

}
void PictureThread::SendPictureSlot()
{
    newConnect(SendPictureToServer);
}

void PictureThread::GOGOGO()
{
    newConnect(tcpSocketPicture);

}
void PictureThread::sendPictureToServerMessage()
{
    SendTcpMessage("14",SendPictureToServer);
    SendTcpMessage(QString::number(PictrueType),SendPictureToServer);
    SendTcpMessage(ServerFileName,SendPictureToServer);

    startTransfer();            //图片信息
    connect(SendPictureToServer,SIGNAL(bytesWritten(qint64)),this,SLOT(sendFile(qint64)));
}

void PictureThread::sendMessage()
{
    qDebug()<<"你大爷个棒棒鸡";
    SendTcpMessage(QString::number(Number),tcpSocketPicture);
    SendTcpMessage(PictureName,tcpSocketPicture);
}
void PictureThread::receiveFile() 	//从服务器获取推荐的图片
{
    QDataStream in(tcpSocketPicture);
    in.setVersion(QDataStream::Qt_5_8);
    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
         if((tcpSocketPicture->bytesAvailable() >= sizeof(qint64)*2)&& (fileNameSize == 0))
         { //接收数据总大小信息和文件名大小信息
             in >> totalBytes >> fileNameSize;
             bytesReceived += sizeof(qint64) * 2;
         }
         if((tcpSocketPicture->bytesAvailable() >= fileNameSize)
             && (fileNameSize != 0))
         {  //接收文件名，并建立文件
             in >> fileName;
             bytesReceived += fileNameSize;


             QDir dirpath;//新建文件夹
             bool is_dirpath=dirpath.exists(FilePath);        //文件路径是否存在
             if(!is_dirpath)
                dirpath.mkdir(FilePath);
             localFile ->setFileName(FilePath+"/"+fileName);        //接收文件路径



             if(!localFile->open(QFile::WriteOnly))
             {
                 qDebug() << "open file error!";
                 return;
             }
         }
         else return;
    }
    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        bytesReceived += tcpSocketPicture->bytesAvailable();
        inBlock = tcpSocketPicture->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    if(bytesReceived == totalBytes)
    { //接收数据完成时
        qDebug()<<"接收完毕"<<fileName;

        tcpSocketPicture->close();
        localFile->close();

        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        qDebug()<<"图片地址asdasdasdasdasdasdasdasdsssssssssssssssssssssssssssssssssssssssssssssssssssss"<<PictureName;
        emit SendFileName(PictureName);                //图片的名字本来就是个编号
    }
}

/*****************************************8给服务器发送照片********************************/
void PictureThread::startTransfer()  //实现文件大小等信息的发送
{


    localFile->setFileName(FilePath);

    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_8);
    QString currentFileName = FilePath.right(FilePath.size() - FilePath.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    bytesToWrite = totalBytes - SendPictureToServer->write(outBlock);
    //发送完头数据后剩余数据的大小
    outBlock.resize(0);
    qDebug()<<"图片地址"<<FilePath<<currentFileName;

}

void PictureThread::sendFile(qint64 numBytes) //更新进度条，实现文件的传送
{
    bytesWritten += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)SendPictureToServer->write(outBlock);
       //发送完一次数据后还剩余数据的大小
        outBlock.resize(0);
        //清空发送缓冲区
    }
    else
    {
        localFile->close(); //如果没有发送任何数据，则关闭文件
    }

    if(bytesWritten == totalBytes) //发送完毕
    {
        qDebug()<<"发送结束";
        localFile->close();
        SendPictureToServer->close();


    }
}


PictureThread::~PictureThread()
{
    qDebug()<<"西沟";
}
