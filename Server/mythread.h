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
class OnlinePeople
{
public:
    QTcpSocket *tcpSocketOnline;            //在线人套接字
    QString UserAccount;
    OnlinePeople(QTcpSocket *tcpSocketOnline_,QString UserAccount_)
    {
        tcpSocketOnline=tcpSocketOnline_;       //存储在线人套接字
        UserAccount=UserAccount_;               //使用者账号
    }
};
class ChatUser
{
public:
    QTcpSocket *tcpSocketChatUser;            //在线人套接字
    QString UserAccount;
    ChatUser(QTcpSocket *tcpSocketChatUser_,QString UserAccount_)
    {
        tcpSocketChatUser=tcpSocketChatUser_;
        UserAccount=UserAccount_;
    }
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
    int Number;                 //数量
    QString Synopsis;               //简介
    QString MassUnit;           //质量单位
    QString PictureLocation;      //图片位置
    float Postage;                    //邮费
    QString Promulgator;            //发送者账号
    QString ContactNumber;          //发布者电话
    Procuct()
    {

    }

    Procuct( QString CommodityCode_s,QString procuct_s,float Cost_s,int Number_s,QString MassUnit_s,QString Synopsis_s,QString PictureLocation_s,float Postage_s,QString Promulgator_s,QString ContactNumber_s)
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

class MyThread : public QThread
{
    Q_OBJECT
signals:
    void OnlinePeopleAccount(OnlinePeople);     //用户信号
    void OffLinePeopleNumber(int);         //下线用户数字
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
};

#endif // MYTHREAD_H
