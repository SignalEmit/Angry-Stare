#ifndef MYTHREAD_H
#define MYTHREAD_H

#include<QThread>
#include<QObject>

#include<QtNetWork>
#include<QByteArray>
#include<QDateTime>
#include<QTcpSocket>
#include<QTcpServer>
#include<QFile>
#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QList>
#include<QMap>
class OnlinePeople
{
private:
    QTcpSocket *tcpSocketOnline;            //在线人套接字
    QTcpSocket *tcpSocketChat;
public:
   QString UserAccount;
    OnlinePeople(QTcpSocket *tcpSocketOnline_,QString UserAccount_):tcpSocketOnline(tcpSocketOnline_),UserAccount(UserAccount_){}
    void setChatSocket(QTcpSocket *tcpSocketChat_){tcpSocketChat=tcpSocketChat_;}
    QTcpSocket *BackChatTcpSocket(){return tcpSocketChat;}
    QTcpSocket *BackOnline(){return tcpSocketOnline;}
};
class newPeople
{
public:
    QString FriendCount;        //朋友账号
    QString Age;            //年龄
    QString NickName;           //昵称
    QString Sex;
    QString Area;
    bool NoYesFriend;
public:
    newPeople(){}
    newPeople(QString FriendCount_,QString Age_,QString NickName_,QString Sex_,QString Area_,bool NoYesFriend_)
    {
        FriendCount=FriendCount_;
        Age=Age_;
        NickName=NickName_;
        Sex=Sex_;
        Area=Area_;
        NoYesFriend=NoYesFriend_;
    }
};
class Friend
{
public:
    QString FriendCount;        //朋友账号
    QString Remark;             //备注
    QString Age;            //年龄
    QString NickName;           //昵称
    QString Sex;
    QString Area;
    bool NoYesFriend;
    QString FriendPictureSize;            //好友照片大小
public:
    Friend(){}
    Friend(QString FriendCount_,QString Remark_,QString Age_,QString NickName_,QString Sex_,QString Area_,bool NoYesFriend_)
    {
        FriendCount=FriendCount_;
        Remark=Remark_;
        Age=Age_;
        NickName=NickName_;
        Sex=Sex_;
        Area=Area_;
        NoYesFriend=NoYesFriend_;
    }
};
class Procuct
{
public:
    QString CommodityCode;          //商品编号
    QString ProductName;            //商品名称

    float Cost;                   //商品价格
    float Number;                 //数量
    QString Synopsis;               //简介
    QString MassUnit;           //质量单位
    QString PictureLocation;      //图片位置
    float Postage;                    //邮费
    QString Promulgator;            //发送者账号
    QString ContactNumber;          //发布者电话
    Procuct()
    {

    }

    Procuct( QString CommodityCode_s,QString procuct_s,float Cost_s,float Number_s,QString MassUnit_s,QString Synopsis_s,QString PictureLocation_s,float Postage_s,QString Promulgator_s,QString ContactNumber_s)
    {
        CommodityCode=CommodityCode_s;
        ProductName=procuct_s;
        Cost=Cost_s;
        Number=Number_s;
        Synopsis=Synopsis_s;
        MassUnit=MassUnit_s;
        PictureLocation=PictureLocation_s;
        Postage=Postage_s;
        Promulgator=Promulgator_s;
        ContactNumber=ContactNumber_s;
    }
};
class PayMessage:public Procuct
{
public:
    QString OrderCommodityCode;
    QString PayGoodsMassUnit;
    int PayGoodsNumber;
    float PayGoodsMoney;
public:
    PayMessage(QString OrderCommodityCode_,QString PayGoodsMassUnit_,int PayGoodsNumber_,float PayGoodsMoney_,
                QString CommodityCode_s,QString procuct_s,float Cost_s,float Number_s,QString MassUnit_s,
               QString Synopsis_s,QString PictureLocation_s,float Postage_s,QString Promulgator_s,QString ContactNumber_s):
        Procuct(CommodityCode_s,procuct_s,Cost_s,Number_s,MassUnit_s,Synopsis_s,PictureLocation_s,Postage_s,Promulgator_s,ContactNumber_s)
    {
        OrderCommodityCode=OrderCommodityCode_;
        PayGoodsMassUnit=PayGoodsMassUnit_;
        PayGoodsNumber=PayGoodsNumber_;
        PayGoodsMoney=PayGoodsMoney_;
    }

};
class MyThread : public QThread
{
    Q_OBJECT
signals:
    void OnlinePeopleAccount(OnlinePeople);     //用户信号
    void OffLinePeopleNumber(QString);         //下线用户数字
    void deleteThread(int);                 //客户端确认收到消息回发 退出线程
private:
    QString Judge;                      //用于判断做什么操作
    QString RandCommodityCode();                           //随机产生一个编号
    QString ServerPictureNumber;                  //设置好友头像
    enum PicTureType{HeadPicture,FarmProduct};
public:
    ~MyThread();
    MyThread();
    MyThread(QTcpServer *tcpServer,QList<Procuct> procuctlist,int threadNumber);
protected:
    void run();





    /***********************数据库数据*********************/
    /***********************数据库数据*********************/
    /***********************数据库数据*********************/
private:
    QSqlDatabase db;


    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
private slots:
    void ggggggggggg();
    void receive_information();
    void sendFile(qint64 numBytes);
    void displayError(QAbstractSocket::SocketError);
    void receiveFile(); 	//从客户端获取推荐的图片
private:
    bool is_receivePicture;                     //是否切换接受图片
    int ThreadNumber;
    void startTransfer(QString filePath);
    void receiveTcpMessage(QString &Judge,QTcpSocket *tcpSocket);
    void SendTcpMessage_List(QTcpSocket *tcpSocket_1,Procuct message);
    void SendTcpMessage_FriendList(QTcpSocket *tcpSocket_1);
    void SendTcpMessage(QString message, QTcpSocket *tcpSocket_1);
    void receiveTcpMessage_FarmPruduct(Procuct &_list,QTcpSocket *tcpSocket);           //接收农业类
    void SendTcpPaySuccessGoods(PayMessage &GoodsMessage,QTcpSocket *tcpSocket);          //发送购买的信息
    void OpenMessageDatabase();                 //基本信息数据库
    quint16 blockSize;
    QTcpSocket *receiveSocket;
    QList<Procuct> ProcuctList;
    QList<Friend> FriendList;

private:
    int SendRecommendPictureCount;          //发送推荐列表图片的计数器
    void SendSellerMessage();               //发送销售者信息
    void ProductPicture();                  //产品图片
    void SendSearchMessage();               //向客户端发送查找到的信息
    void SearchProduct();                   //查询产品
    void AddFriendInDatabase();             //添加好友入数据库
    void SearchFriend();                    //添加好友
    void ReceiveSendChatMessage();          //接收并发送信息
    void receiveChatSocket();               //接收聊天界面的套接字
    void OffLine();                         //下线
    void SaveOnlinePeople();                //存储上线好友套接字和姓名
    void GetFriendList();                   //获取好友列表
    void RecommendPicture();                //推荐图片
    void ForgetPassword();                  //忘记密码
    void ComeIntoInterface();               //进入主界面
    void CheckAccountRegisterLogIn();       //检测账号
    void RegisterLogIn();                   //注册账号
    void ReadRecommendList();               //读取推荐列表
    void FriendPicture();                   //朋友图片
    void ReceiveManyPicture();              //接收来自客户端的各种各样的图片
    void AddFarmProduct();                  //给农产品表添加新条目
    void CheckFileSize();                   //核对各种文件的大小用以不浪费流量 不重复接收
    void PayGoodsMoney();                   //支付了购买商品的价钱
    void SelectMyPaySuccessGoods();         //查找并发送支付成功的商品
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    /***********************8网络数据*********************/
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区


    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString JudgeStockClose;        //套接字是否关闭
    int PictureDir;                //各种图片的文件夹路径

private:
friend QDataStream& operator<<(QDataStream &out, Procuct &data);


friend QDataStream& operator<<(QDataStream &out, Friend &data);

friend QDataStream& operator<<(QDataStream &out, newPeople &data);
friend QDataStream& operator<<(QDataStream &out, PayMessage &data);
};

#endif // MYTHREAD_H
