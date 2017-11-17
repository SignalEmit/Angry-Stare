#include "mythread.h"
#include <QTableView>
#include <QSqlTableModel>
extern QMap<QString,OnlinePeople> OnlinePeopleMap;
MyThread::MyThread()
{

}
MyThread::MyThread(QTcpServer *tcpServer,QList<Procuct> procuctlist,int threadNumber)
{
    is_receivePicture=false;                //是否切换接受图片
    qDebug()<<"第"<<threadNumber<<"条线程";
    ThreadNumber=threadNumber;
    SendRecommendPictureCount=0;
    /******************************************/
    loadSize = 4*1024;
    bytesWritten = 0;
    bytesToWrite = 0;
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    /******************************************/
    ProcuctList=procuctlist;                //链表赋值全局使用
    localFile = new QFile;
    receiveSocket = tcpServer->nextPendingConnection();
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Message.db");
}
MyThread::~MyThread()
{
    if(Judge.toInt()!=8 && Judge.toInt()!=10)
        delete receiveSocket;
    delete localFile;
    ProcuctList.clear();
    FriendList.clear();

}
void MyThread::run()
{
    connect(receiveSocket, SIGNAL(readyRead()), this, SLOT(receive_information()));
    connect(receiveSocket,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}
void MyThread::displayError(QAbstractSocket::SocketError)
{
    if(JudgeStockClose=="发送朋友消息")
    {
            qDebug()<<"断开了";
    }
}

void MyThread::receive_information()
{
    if(!is_receivePicture)
    {
        receiveTcpMessage(Judge,receiveSocket);
        switch (Judge.toInt()) {
            case 0:ComeIntoInterface();break;               //接收登录
            case 1:CheckAccountRegisterLogIn();break;       //接收检测账号
            case 2:RegisterLogIn();break;                   //接收注册账号
            case 3:ForgetPassword();break;                  //接收忘记密码
            case 4:ReadRecommendList();break;               //接收推荐列表
            case 5:RecommendPicture();break;                //发送商品图片
            case 6:GetFriendList();break;                   //接收发送朋友列表
            case 7:FriendPicture();break;                   //发送朋友图片
            case 8:SaveOnlinePeople();break;                //存储上线好友的套接字
            case 9:OffLine();break;                         //下线
            case 10:receiveChatSocket();break ;             //接收聊天界面的套接字                  //聊天线程没被释放掉
            case 11:ReceiveSendChatMessage();break;         //接收并发送信息
            case 12:SearchFriend();break;                   //添加好友
            case 13:AddFriendInDatabase();break;            //将好友加入数据库中
            case 14:ReceiveManyPicture();break;             //接收来自客户端的各.种各样的图片
            case 15:AddFarmProduct();break;                 //给农产品表添加新条目
            case 16:SearchProduct();break;                  //查询产品
            case 17:SendSearchMessage();break;              //向客户端发送查找到的信息
            case 18:ProductPicture();break;                 //产品图片
            case 19:SendSellerMessage();break;              //发送销售者账号信息
            case 20:CheckFileSize();break;                  //核对各种文件的大小用以不浪费流量 不重复接收
            case 21:PayGoodsMoney();break;                  //支付了购买商品的价钱 加入数据库中
        default:
            break;
        }
    }
    else
        receiveFile();
}
void MyThread::PayGoodsMoney()
{
    QString PayUserAccount,PromulgatorAccount,PayCommodityCode,PayGoodsMassUnit,PayGoodsNumber,PayGoodsMoney;      //支付者账号，商品拥有着账号,商品编号，单位，购买的数量，购买的钱数
    receiveTcpMessage(PayUserAccount,receiveSocket);
    receiveTcpMessage(PromulgatorAccount,receiveSocket);
    receiveTcpMessage(PayCommodityCode,receiveSocket);
    receiveTcpMessage(PayGoodsMassUnit,receiveSocket);
    receiveTcpMessage(PayGoodsNumber,receiveSocket);
    receiveTcpMessage(PayGoodsMoney,receiveSocket);
    db.open();
    QSqlQuery CheckGoodsNumber;
    CheckGoodsNumber.exec(tr("select MassUnit from FarmProductsMessage where CommodityCode='%0'").arg(PayCommodityCode));
    float Minus=0;                        //减数
    if(CheckGoodsNumber.next())             //如果商品存在
    {
        if(PayGoodsMassUnit=="斤")
        {
            if(CheckGoodsNumber.value("MassUnit").toString()=="斤")
                Minus=(float)PayGoodsNumber.toInt();
            else if(CheckGoodsNumber.value("MassUnit").toString()=="千克")
                Minus=(float)PayGoodsNumber.toInt()/2;
            else if(CheckGoodsNumber.value("MassUnit").toString()=="吨" )
                Minus=(float)PayGoodsNumber.toInt()/2000;
        }
        else if(PayGoodsMassUnit=="千克")
        {
            if(CheckGoodsNumber.value("MassUnit").toString()=="斤")
                Minus=(float)PayGoodsNumber.toInt()*2;
            else if(CheckGoodsNumber.value("MassUnit").toString()=="千克")
                Minus=(float)PayGoodsNumber.toInt();
            else if(CheckGoodsNumber.value("MassUnit").toString()=="吨")
                Minus=(float)PayGoodsNumber.toInt()/1000;
        }
        else
        {
            if(CheckGoodsNumber.value("MassUnit").toString()=="斤")
                Minus=(float)PayGoodsNumber.toInt()*2000;
            else if(CheckGoodsNumber.value("MassUnit").toString()=="千克")
                Minus=(float)PayGoodsNumber.toInt()*1000;
            else
                Minus=(float)PayGoodsNumber.toInt();
        }
    }
    qDebug()<<"价钱是:"<<Minus;
    QSqlQuery MinusOperation;                           //做减操作
    if(MinusOperation.exec(tr("update FarmProductsMessage set Number=Number-%0 where CommodityCode='%1'").arg(Minus).arg(PayCommodityCode)))
    {
        QString PaySuccessfullyCommodityCode;
        QSqlQuery AddPayMessage;
        do{
            PaySuccessfullyCommodityCode=RandCommodityCode();
        }
        while(!AddPayMessage.exec(tr("insert into PaySuccessfullyProducts(OrderCommodityCode,PayUserAccount,PayCommodityCode,PayGoodsMassUnit,PayGoodsNumber,PayGoodsMoney) values('%0','%1','%2','%3',%4,%5)").arg(RandCommodityCode()).arg(PayUserAccount).arg(PayCommodityCode).arg(PayGoodsMassUnit).arg(PayGoodsNumber.toInt()).arg(PayGoodsMoney.toFloat())));
        SendTcpMessage("购买成功",receiveSocket);
        SendTcpMessage(PaySuccessfullyCommodityCode,receiveSocket);
    }
    else
        SendTcpMessage("物品所剩数量不足",receiveSocket);
    db.close();

    receiveSocket->disconnectFromHost();
}

void MyThread::CheckFileSize()
{
    QFile pictruesize;
    QString FileJudge;                  //文件判断
    receiveTcpMessage(FileJudge,receiveSocket);
    if(FileJudge=="Product")
    {
        QString ProductPictureSize;
        receiveTcpMessage(ProductPictureSize,receiveSocket);
        for(int i=0;i<ProductPictureSize.toInt();i++)
        {
            QString ProductNumber;
            receiveTcpMessage(ProductNumber,receiveSocket);
            pictruesize.setFileName("FarmProducts/"+ProductNumber+".jpg");
            SendTcpMessage(QString::number(pictruesize.size()),receiveSocket);
        }
    }
    receiveSocket->disconnectFromHost();
}

void MyThread::ProductPicture()
{

    QString ProductCount;
    receiveTcpMessage(ProductCount,receiveSocket);
    startTransfer("FarmProducts/"+ProductCount+".jpg");
    connect(receiveSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(sendFile(qint64)));
}

void MyThread::SendSearchMessage()
{
    QString SearchString;     //查询字符串
    receiveTcpMessage(SearchString,receiveSocket);
    db.open();
    QSqlQuery AddCount;                     //统计个数
    AddCount.exec(tr("select  count(*) from FarmProductsMessage where ProductName like '%0'").arg("%"+SearchString+"%"));
    if(AddCount.next())
    {
        SendTcpMessage(QString::number(AddCount.value(0).toInt()),receiveSocket);           //发送查出来的个数


        QSqlQuery CheckProduct;
        CheckProduct.exec(tr("select  * from FarmProductsMessage where ProductName like '%0'").arg("%"+SearchString+"%"));
        while(CheckProduct.next())
        {
            Procuct newProduct(CheckProduct.value(0).toString(),CheckProduct.value(1).toString(),CheckProduct.value(2).toFloat(),CheckProduct.value(3).toFloat(),CheckProduct.value(4).toString(),CheckProduct.value(5).toString(),CheckProduct.value(6).toString(),CheckProduct.value(7).toFloat(),CheckProduct.value(8).toString(),CheckProduct.value(9).toString());
                    //新产品
            SendTcpMessage_List(receiveSocket,newProduct);              //发送产品
        }
    }
    db.close();

    receiveSocket->disconnectFromHost();
}

void MyThread::SearchProduct()
{
    QString SearchString;     //查询字符串
    receiveTcpMessage(SearchString,receiveSocket);
    db.open();
    QSqlQuery AddCount;                     //统计个数
    AddCount.exec(tr("select  count(*) from FarmProductsMessage where ProductName like '%0'").arg("%"+SearchString+"%"));
    if(AddCount.next())
    {
        SendTcpMessage(QString::number(AddCount.value(0).toInt()),receiveSocket);
        qDebug()<<"查到的值"<<QString::number(AddCount.value(0).toInt());
        QSqlQuery CheckProduct;
        CheckProduct.exec(tr("select  ProductName from FarmProductsMessage where ProductName like '%0'").arg("%"+SearchString+"%"));
        while(CheckProduct.next())
        {
            SendTcpMessage(CheckProduct.value(0).toString(),receiveSocket);
        }
    }
    db.close();

    receiveSocket->disconnectFromHost();
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

void MyThread::receiveTcpMessage_FarmPruduct(Procuct &_list,QTcpSocket *tcpSocket)
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



QString MyThread::RandCommodityCode()                           //随机产生一个编号
{
    QDateTime time = QDateTime::currentDateTime();

    QTime RandSeed;
    RandSeed = QTime::currentTime();
    qsrand(RandSeed.msec() + RandSeed.second() * 1000);

    int RandNumber=qrand()%8999+1000;

    return time.toString("yyyyMMddHHmmss")+QString::number(RandNumber);
}

void MyThread::AddFarmProduct()
{
    Procuct newFarmProduct;
    receiveTcpMessage_FarmPruduct(newFarmProduct,receiveSocket);
    qDebug()<<newFarmProduct.ProductName<<newFarmProduct.CommodityCode<<newFarmProduct.Cost<<newFarmProduct.Number<<newFarmProduct.Synopsis<<newFarmProduct.MassUnit<<newFarmProduct.PictureLocation<<newFarmProduct.Postage<<newFarmProduct.Promulgator<<newFarmProduct.ContactNumber;          //发布者电话;

   newFarmProduct.CommodityCode=RandCommodityCode();                //产生编号
   newFarmProduct.PictureLocation="FarmProducts/"+newFarmProduct.CommodityCode+".jpg";              //图片位置

   qDebug()<<newFarmProduct.CommodityCode;
    db.open();
   QSqlQuery UpdataFarmProduct;
   while(1)
   {
       if(UpdataFarmProduct.exec(QString("insert into FarmProductsMessage(CommodityCode,ProductName,Cost,Number,MassUnit,Synopsis,PictureLocation,Postage,Promulgator,ContactNumber) values('%0','%1',%2,%3,'%4','%5','%6',%7,'%8','%9')").arg(newFarmProduct.CommodityCode).arg(newFarmProduct.ProductName).arg(newFarmProduct.Cost).arg(newFarmProduct.Number).arg(newFarmProduct.MassUnit).arg(newFarmProduct.Synopsis).arg(newFarmProduct.PictureLocation).arg(newFarmProduct.Postage).arg(newFarmProduct.Promulgator).arg(newFarmProduct.ContactNumber)))
           break;
       else
       {
           newFarmProduct.CommodityCode=RandCommodityCode();
           newFarmProduct.PictureLocation="FarmProducts/"+newFarmProduct.CommodityCode+".jpg";
       }
   }
   db.close();

   is_receivePicture=true;
   SendTcpMessage(newFarmProduct.CommodityCode,receiveSocket);              //发回编号接收图片  接到农产品列表
}

void MyThread::ReceiveManyPicture()
{
    QString JudgeTranslate;
    receiveTcpMessage(JudgeTranslate,receiveSocket);
    receiveTcpMessage(ServerPictureNumber,receiveSocket);
    qDebug()<<"他妈外棒棒鸡他妈外棒棒鸡他妈外棒棒鸡他妈外棒棒鸡";
    PictureDir=JudgeTranslate.toInt();
    is_receivePicture=true;
}

void MyThread::AddFriendInDatabase()
{
    QString UserAccount,FriendAccount;
    receiveTcpMessage(UserAccount,receiveSocket);
    receiveTcpMessage(FriendAccount,receiveSocket);
    db.open();
    QSqlQuery addfriendindatabase;      //添加好友入数据库
    addfriendindatabase.exec(tr("insert into FriendMessage(MyAccout,FriendAccout) values('%0','%1')").arg(UserAccount,FriendAccount));
    db.close();
    SendTcpMessage("添加成功",receiveSocket);

    receiveSocket->disconnectFromHost();
}

QDataStream& operator<<(QDataStream &out, newPeople &data)
{
    out<<data.FriendCount<<data.Age<<data.NickName<<data.Sex<<data.Area<<data.NoYesFriend;
    return out;
}
void MyThread::SearchFriend()
{
    QString UserAccount,FriendAccount;
    receiveTcpMessage(UserAccount,receiveSocket);
    receiveTcpMessage(FriendAccount,receiveSocket);
    qDebug()<<FriendAccount<<UserAccount;
    db.open();
    QSqlQuery Search;       //查询好友
    Search.exec(tr("select * from FriendMessage where MyAccout='%0' and FriendAccout='%1'").arg(UserAccount,FriendAccount));
    if(Search.next())
        SendTcpMessage("好友已存在",receiveSocket);
    else
    {
        QSqlQuery searchNewFriendAccount;
        searchNewFriendAccount.exec(tr("select Account,Age,Nickname,Sex,Area from UserMessage where Account='%0'").arg( FriendAccount));
        if(searchNewFriendAccount.next())
        {
            SendTcpMessage("有用户",receiveSocket);
            newPeople oneFriend(searchNewFriendAccount.value(0).toString(),searchNewFriendAccount.value(1).toString(),searchNewFriendAccount.value(2).toString(),searchNewFriendAccount.value(3).toString(),searchNewFriendAccount.value(4).toString(),false);
            /*********************************发送对象********************************/
            QByteArray data;			//暂时要发送的数据
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_8);						//设置数据流版本
            out << (quint16)0;
            out << oneFriend;
            out.device()->seek(0);
            out << (quint16)(data.size() - sizeof(quint16));
            receiveSocket->write(data);

            /*********************************发送对象********************************/
        }
        else
            SendTcpMessage("无此用户",receiveSocket);
    }
    db.close();

    receiveSocket->disconnectFromHost();
}

void MyThread::ReceiveSendChatMessage()              //接收并发送信息
{

    QString SendUserAccount,FriendAccount,chatMessage;      //发送者账号，朋友账号，聊天信息
    receiveTcpMessage(SendUserAccount,receiveSocket);
    receiveTcpMessage(FriendAccount,receiveSocket);
    receiveTcpMessage(chatMessage,receiveSocket);
    QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(FriendAccount);
    if(LookUp!=OnlinePeopleMap.end())
    {
        JudgeStockClose="发送朋友消息";
        qDebug()<<"什么情况";
        //connect(OnlinePeopleList.at(i).BackOnline(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
        SendTcpMessage(SendUserAccount,LookUp.value().BackChatTcpSocket());
        SendTcpMessage(chatMessage,LookUp.value().BackChatTcpSocket());
    }
}
void MyThread::receiveChatSocket()
{
    QString UserAccount;
    receiveTcpMessage(UserAccount,receiveSocket);
    QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(UserAccount);
    if(LookUp!=OnlinePeopleMap.end())
    {
          LookUp.value().setChatSocket(receiveSocket);
    }
    qDebug()<<"还有几条map："<<OnlinePeopleMap.size();
    /*************************8没清*******************************************/
    /*************************8没清*******************************************/
    /*************************8没清*******************************************/
}
void MyThread::OffLine()
{
    QString UserAccount;
    receiveTcpMessage(UserAccount,receiveSocket);
    QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(UserAccount);
    if(LookUp!=OnlinePeopleMap.end())
    {
        emit OffLinePeopleNumber(UserAccount);
        SendTcpMessage("下线吧",receiveSocket);
    }

    receiveSocket->disconnectFromHost();

}

void MyThread::SaveOnlinePeople()
{
    QString UserAccount;
    receiveTcpMessage(UserAccount,receiveSocket);
    QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(UserAccount);
    if(LookUp!=OnlinePeopleMap.end())
    {
        SendTcpMessage("异地登录",LookUp.value().BackOnline());
        emit OffLinePeopleNumber(UserAccount);
    }
    OnlinePeople onlinepeople(receiveSocket,UserAccount);       //建立对象
    emit OnlinePeopleAccount(onlinepeople);                      //发送对象
    SendTcpMessage("准备接收聊天界面套接字",receiveSocket);
    qDebug()<<"还有几条map："<<OnlinePeopleMap.size();

     /*************************8没清*******************************************/
     /*************************8没清*******************************************/
     /*************************8没清*******************************************/
}

void MyThread::FriendPicture()
{
    qDebug()<<"进入7";
    QString FriendCount;
    receiveTcpMessage(FriendCount,receiveSocket);
    startTransfer("HeadPicture/"+FriendCount+".jpg");
    connect(receiveSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(sendFile(qint64)));
}

void MyThread::GetFriendList()
{
    QFile FriendPictureSize;
    QString UserAccount;            //用户账号
    receiveTcpMessage(UserAccount,receiveSocket);
    db.open();
    QSqlQuery searchFriendCount;
    searchFriendCount.exec(tr("select count(FriendAccout) from FriendMessage where MyAccout='%0'").arg(UserAccount));
    if(searchFriendCount.next())
        SendTcpMessage(searchFriendCount.value(0).toString(),receiveSocket);
    QSqlQuery searchFriendList;
    searchFriendList.exec(tr("select FriendAccout,Remark from FriendMessage where MyAccout='%0'").arg(UserAccount));
    while(searchFriendList.next())
    {
        QSqlQuery searchFriendMessage;              //查询好友信息
        searchFriendMessage.exec(tr("select Account,Age,Nickname,Sex,Area from UserMessage where Account='%0'").arg( searchFriendList.value(0).toString()));
        searchFriendMessage.next();
        Friend oneFriend(searchFriendMessage.value(0).toString(),searchFriendList.value(1).toString(),searchFriendMessage.value(1).toString(),searchFriendMessage.value(2).toString(),searchFriendMessage.value(3).toString(),searchFriendMessage.value(4).toString(),true);
        FriendPictureSize.setFileName("HeadPicture/"+searchFriendMessage.value(0).toString()+".jpg");
        oneFriend.FriendPictureSize=QString::number(FriendPictureSize.size());
        FriendList.push_back(oneFriend);

    }
    db.close();
    SendTcpMessage_FriendList(receiveSocket);

    receiveSocket->disconnectFromHost();
}

void MyThread::RecommendPicture()
{
    QString s;
    receiveTcpMessage(s,receiveSocket);
    SendRecommendPictureCount=s.toInt();
    qDebug()<<"读取哪个未知的"<<SendRecommendPictureCount<<ProcuctList.at(SendRecommendPictureCount).PictureLocation;
    startTransfer(ProcuctList.at(SendRecommendPictureCount).PictureLocation);
    connect(receiveSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(sendFile(qint64)));
}

void MyThread::ReadRecommendList()
{
    SendTcpMessage(QString::number(ProcuctList.size()),receiveSocket);      //发送商品个数

    for(int i=0;i<ProcuctList.size();i++)
        SendTcpMessage_List(receiveSocket,ProcuctList.at(i));

    receiveSocket->disconnectFromHost();

}

/*******************忘记密码********************/
void MyThread::ForgetPassword()
{
    QString Account,Password,PhoneNumber;
    receiveTcpMessage(Account,receiveSocket);
    receiveTcpMessage(Password,receiveSocket);
    receiveTcpMessage(PhoneNumber,receiveSocket);
    db.open();
    QSqlQuery forgetpassword;      //忘记密码
    forgetpassword.exec(tr("select Phone from UserMessage where Account='%0'").arg(Account));
    if(forgetpassword.next())
    {
        if(PhoneNumber!=forgetpassword.value(0))
        {
            SendTcpMessage("手机号错误",receiveSocket);
            db.close();
        }
        else
        {
            forgetpassword.exec(tr("update UserMessage set Password='%0' where Account='%1'").arg(Password).arg(Account));
            SendTcpMessage("修改成功",receiveSocket);
            db.close();
        }
    }
    else
    {
        SendTcpMessage("账号不存在",receiveSocket);
        db.close();
    }

    receiveSocket->disconnectFromHost();
}

/*******************注册账号********************/
void MyThread::RegisterLogIn()
{
    QString Account,Password,PhoneNumber;
    receiveTcpMessage(Account,receiveSocket);
    receiveTcpMessage(Password,receiveSocket);
    receiveTcpMessage(PhoneNumber,receiveSocket);
    db.open();
    QSqlQuery RegisterAccount;      //注册账号
    if(RegisterAccount.exec(tr("insert into UserMessage(Account,Password,Phone) values('%0','%1','%2')").arg(Account).arg(Password).arg(PhoneNumber)))
        SendTcpMessage("注册成功",receiveSocket);
    else
        SendTcpMessage("账号存在",receiveSocket);
    db.close();
    receiveSocket->disconnectFromHost();
}

/*******************检查账号是否存在********************/
void MyThread::CheckAccountRegisterLogIn()
{
    QString Account;
    receiveTcpMessage(Account,receiveSocket);
    db.open();
    QSqlQuery CheckAccount;
    CheckAccount.exec(tr("select * from UserMessage where Account='%0'").arg(Account));
    if(CheckAccount.next())
        SendTcpMessage("账号存在",receiveSocket);
    else
        SendTcpMessage("账号不存在",receiveSocket);
    db.close();

    receiveSocket->disconnectFromHost();


}
/*******************登录********************/
void MyThread::ComeIntoInterface()
{
    QString Account,Password;                           //定义账号密码
    receiveTcpMessage(Account,receiveSocket);           //接收账号
    receiveTcpMessage(Password,receiveSocket);          //接收密码


    db.open();
    QSqlQuery seek_information;                 //核对账号密码
    seek_information.exec(tr("select * from UserMessage where Account='%0' and Password='%1'").arg(Account).arg(Password));
    if (seek_information.next())
        SendTcpMessage("密码账号正确",receiveSocket);
    else
        SendTcpMessage("密码账号不正确",receiveSocket);
    db.close();



    receiveSocket->disconnectFromHost();

}


/******************************8接收消息*******************************/
void MyThread::receiveTcpMessage(QString &Judge,QTcpSocket *tcpSocket)
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
    qDebug()<<Judge;

}
/******************************发送消息*******************************/
void MyThread::SendTcpMessage(QString message,QTcpSocket *tcpSocket_1)
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


QDataStream& operator<<(QDataStream &out, Procuct &data)
{
    out<<data.ProductName<<data.CommodityCode<<data.Cost<<data.Number<<data.Synopsis<<data.MassUnit<<data.PictureLocation<<data.Postage<<data.Promulgator<<data.ContactNumber;
    return out;
}

/******************************发送对象*******************************/
void MyThread::SendTcpMessage_List(QTcpSocket *tcpSocket_1,Procuct message)
{
        QByteArray data;			//暂时要发送的数据
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);						//设置数据流版本
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(data.size() - sizeof(quint16));
        tcpSocket_1->write(data);

}

QDataStream& operator<<(QDataStream &out, Friend &data)
{
    out<<data.FriendCount<<data.Remark<<data.Age<<data.NickName<<data.Sex<<data.Area<<data.NoYesFriend<<data.FriendPictureSize;
    return out;
}
/******************************发送朋友列表*******************************/
void MyThread::SendTcpMessage_FriendList(QTcpSocket *tcpSocket_1)
{
    for(int i=0;i<FriendList.size();i++)
    {
     // Procuct message(ProcuctList.at(i).ProductName,ProcuctList.at(i).CommodityCode,ProcuctList.at(i).Cost,ProcuctList.at(i).Number,ProcuctList.at(i).Synopsis,ProcuctList.at(i).MassUnit,ProcuctList.at(i).PictureLocation,ProcuctList.at(i).Postage,ProcuctList.at(i).Promulgator,ProcuctList.at(i).ContactNumber);
        Friend message=FriendList.at(i);
      // qDebug()<<FriendList.size()<<message.CommodityCode<<message.ContactNumber<<message.Cost<<message.MassUnit<<message.Number<<message.PictureLocation<<message.Postage<<message.ProductName<<message.Promulgator<<message.Synopsis;

        QByteArray data;			//暂时要发送的数据
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);						//设置数据流版本
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(data.size() - sizeof(quint16));
        tcpSocket_1->write(data);
   }

}

void MyThread::ggggggggggg()
{
    qDebug()<<"俄asdasdaszxczxcadssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";
}

/*****************************服务器向客户端发送文件************************/
void MyThread::startTransfer(QString filePath)  //实现文件大小等信息的发送
{

    QString fileName=filePath;
    qDebug()<<fileName;

    QFile fileExits(fileName);
    if(fileExits.exists())
        localFile->setFileName(fileName);
    else
        localFile->setFileName("默认头像.jpg");

    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_8);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    bytesToWrite = totalBytes - receiveSocket->write(outBlock);
    //发送完头数据后剩余数据的大小
    outBlock.resize(0);

}

void MyThread::sendFile(qint64 numBytes) //更新进度条，实现文件的传送
{
    bytesWritten += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)receiveSocket->write(outBlock);
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
        receiveSocket->close();
        receiveSocket->disconnectFromHost();

    }
}
void MyThread::receiveFile() 	//从客户端获取推荐的图片
{
    QDataStream in(receiveSocket);
    in.setVersion(QDataStream::Qt_5_8);
    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
         if((receiveSocket->bytesAvailable() >= sizeof(qint64)*2)&& (fileNameSize == 0))
         { //接收数据总大小信息和文件名大小信息
             in >> totalBytes >> fileNameSize;
             bytesReceived += sizeof(qint64) * 2;
         }
         if((receiveSocket->bytesAvailable() >= fileNameSize)
             && (fileNameSize != 0))
         {  //接收文件名，并建立文件
             in >> fileName;
             bytesReceived += fileNameSize;

                qDebug()<<"图片大小"<<bytesReceived<<totalBytes;

              if(PictureDir==PicTureType::HeadPicture)
                localFile ->setFileName("HeadPicture/"+fileName);        //接收文件路径
              else
                  localFile ->setFileName("FarmProducts/"+fileName);        //接收文件路径



             if(!localFile->open(QFile::WriteOnly))
             {
                 qDebug() << "open file error!";
                 receiveSocket->disconnectFromHost();
                 return;
             }
         }
         else return;
    }
    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        bytesReceived += receiveSocket->bytesAvailable();
        inBlock = receiveSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    if(bytesReceived == totalBytes)
    { //接收数据完成时
     //   qDebug()<<"接收完毕"<<fileName;

        receiveSocket->close();
        localFile->close();

        if(PictureDir==PicTureType::HeadPicture)
        {
            qDebug()<<"asdawqe2134684313454";
            QFile::remove("HeadPicture/"+ServerPictureNumber+".jpg");
            QFile::rename("HeadPicture/"+fileName,"HeadPicture/"+ServerPictureNumber+".jpg");
        }
        else if(PictureDir==PicTureType::FarmProduct)
        {
            QFile::rename("FarmProducts/"+fileName,"FarmProducts/"+ServerPictureNumber+".jpg");
        }


        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;

        receiveSocket->disconnectFromHost();
    }

}
void MyThread::SendSellerMessage()
{
    QString SellerAccount;                          //销售者账号
    receiveTcpMessage(SellerAccount,receiveSocket);
    QSqlQuery selectSellerMessage;                      //查询销售者信息
    selectSellerMessage.exec(tr("select * from UserMessage where Account='%0'").arg(SellerAccount));
    if(selectSellerMessage.next())
    {
        Friend newfriend(selectSellerMessage.value("Account").toString(),"",selectSellerMessage.value("Age").toString(),selectSellerMessage.value("Nickname").toString(),selectSellerMessage.value("Sex").toString(),selectSellerMessage.value("Area").toString(),false);
        QFile FriendPicture(tr("HeadPicture/%0.jpg").arg(selectSellerMessage.value("Account").toString()));
        newfriend.FriendPictureSize=QString::number(FriendPicture.size());
        QByteArray data;			//暂时要发送的数据
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);						//设置数据流版本
        out << (quint16)0;
        out <<newfriend;
        out.device()->seek(0);
        out << (quint16)(data.size() - sizeof(quint16));
        receiveSocket->write(data);
    }
    receiveSocket->disconnectFromHost();
}
