#include "chat.h"
#include "ui_chat.h"
#include<QDesktopWidget>
#include<QPalette>
#include"dialoglogin.h"
#include<QScroller>
extern int ChatPage;
extern Friend showFriend;
extern QString UserAccount;                //使用者账号
extern QString dialogloginJudge;           //提示界面判断
Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);


    /*********************************数据库*************************/

    scrollArea=new ScrollArea;
    ui->verticalLayout_5->addWidget(scrollArea);

    is_ChatPageShow=false;                                      //最好赋值防止java出错
    ui->page_1->setLayout(ui->verticalLayout_2);            //将竖直排布绑定到page1
    this->setLayout(ui->verticalLayout);                    //绑定整个排布到主界面

    ui->page_2->setLayout(ui->gridLayout);                   //将网格 排布绑定到page2
    ui->label_5->setFixedHeight(5);                             //隔行1

    ui->label_6->setFixedHeight(5);                            //隔行2

    ui->HeadPicture->setFixedSize(width_Change(0.3),width_Change(0.2));          //固定头像的大小




    ui->gridLayout->setContentsMargins(0,0,0,heigth_Change(0.5));

/*******************************************设置上标题****************************/
    ui->Back->setFixedSize(width_Change(0.1),width_Change(0.1));       //设置上标题
    ui->Shop->setFixedSize(width_Change(0.1),width_Change(0.1));
    ui->FriendAccount->setFixedHeight(width_Change(0.1));

/*******************************************设置下标题****************************/
    ui->GetUp->setFixedSize(width_Change(0.15),width_Change(0.15));
    ui->SendMessage->setFixedSize(width_Change(0.15),width_Change(0.15));
    ui->MessagelineEdit->setFixedHeight(width_Change(0.1));


/********************************88设置搜索框**************************************/
    ui->verticalLayout_3->setContentsMargins(0,0,0,heigth_Change(0.9)-width_Change(0.1));
    ui->page_3->setLayout(ui->verticalLayout_3);
    ui->SearchFriendBt->setFixedHeight(heigth_Change(0.05));
    ui->SearchFriendAccount->setFixedHeight(heigth_Change(0.05));

     /*****************************添加好友************************/

    ui->SearchFriendBt->hide();         //隐藏添加好友按钮


    AddFriendTcpSocket=new QTcpSocket(this);
    connect(AddFriendTcpSocket, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(AddFriendTcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(AddFriendTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));



    AddFriendSuccees=new QTcpSocket(this);
    connect(AddFriendSuccees, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(AddFriendSuccees, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(AddFriendSuccees, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));



    ChatWithFriendTcpSocket=new QTcpSocket(this);
    connect(ChatWithFriendTcpSocket, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(ChatWithFriendTcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(ChatWithFriendTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));


    /***********************************滑动区赋值*******************************/

}


void Chat::BuildFilePathWriteChatMessage(QString Judge,QString newMan,QString newChatMessage)
{
    if(Judge=="初始读取消息")
    {
        db.open();
        QSqlQuery findChatMessage;
        findChatMessage.exec(tr("select is_Judgeme,ChatText from %0").arg(newMan));
        qDebug()<<newMan<<"初始读取消息";
        while(findChatMessage.next())
        {
            if(findChatMessage.value("is_Judgeme").toBool())
                scrollArea->SetTextAndPicture(true,UserAccount,findChatMessage.value("ChatText").toString());
            else
                scrollArea->SetTextAndPicture(false,newMan,findChatMessage.value("ChatText").toString());
        }
        db.close();
    }
    else if(Judge=="界面打开但接受的消息并不是这个人的")
    {
        db.open();
        QSqlQuery findChatMessage;
        findChatMessage.exec(tr("insert into %0(is_Judgeme,ChatText) values('false','%1')").arg(newMan,newChatMessage));
        db.close();
    }

}

void Chat::closeEvent(QCloseEvent *e)
{
    is_ChatPageShow=false;                                      //最好赋值防止java出错
    scrollArea->InitializtionScrollArea();
    ui->SearchFriendAccount->clear();
    ui->FriendAccount->clear();
}

void Chat::CreatChatView(QString FriendAccount)
{
    QSqlQuery dataBaseWork;                     //数据库建表
    db.open();
    dataBaseWork.exec(tr("CREATE TABLE [%0]("
                      "[is_Judgeme] BOOL(2),"
                      "[ChatText] VARCHAR(50));").arg(FriendAccount));
    db.close();
}

void Chat::showEvent(QShowEvent *event)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
     QDir dir;
     dir.mkpath(tr("ChatMessage/%0").arg(UserAccount));
    db.setDatabaseName(tr("ChatMessage/%0/ChatDateBase.db").arg(UserAccount));
    is_ChatPageShow=true;                               //聊天界面显示

    ui->stackedWidget->setCurrentIndex(ChatPage);
    ui->HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(showFriend.FriendCount));
    ui->Remarklabel->setText(showFriend.Remark);
    ui->Nicknamelabel->setText("昵称："+showFriend.NickName);
    ui->FriendAccountlabel->setText("账号："+showFriend.FriendCount);
    ui->Arealabel->setText("地区："+showFriend.Area);
    CreatChatView(showFriend.FriendCount);
    if(showFriend.NoYesFriend)
    {
        ui->AddFriendBt->hide();

        qDebug()<<"隐藏";
    }
    else
    {
        ui->AddFriendBt->show();
        qDebug()<<"没有隐藏";

    }
    if(ui->stackedWidget->currentIndex()==0)
    {
        ui->FriendAccount->setText(ui->FriendAccountlabel->text().mid(3));
        BuildFilePathWriteChatMessage("初始读取消息",showFriend.FriendCount);
        qDebug()<<"初始读取消息";
    }

}

Chat::~Chat()
{
    delete ui;
    delete AddFriendTcpSocket;
    delete AddFriendSuccees;
    delete ChatWithFriendTcpSocket;
    delete scrollArea;

}

void Chat::on_Back_clicked()
{
    this->close();
}

void Chat::on_SendMessageBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->FriendAccount->setText(ui->FriendAccountlabel->text().mid(3));
    scrollArea->InitializtionScrollArea();
    BuildFilePathWriteChatMessage("初始读取消息",ui->FriendAccountlabel->text().mid(3));                      //搜素时读取聊天记录
}
void Chat::sendMessage()
{
    if(sender()==ChatWithFriendTcpSocket)
    {
        SendTcpMessage("10",ChatWithFriendTcpSocket);
        SendTcpMessage(UserAccount,ChatWithFriendTcpSocket);
    }
    else if(sender()==AddFriendTcpSocket)
    {
        SendTcpMessage("12",AddFriendTcpSocket);
        SendTcpMessage(UserAccount,AddFriendTcpSocket);
        SendTcpMessage(ui->SearchFriendAccount->text(),AddFriendTcpSocket);
    }
    else if(sender()==AddFriendSuccees)
    {
        SendTcpMessage("13",AddFriendSuccees);
        SendTcpMessage(UserAccount,AddFriendSuccees);
        SendTcpMessage(ui->FriendAccountlabel->text().mid(3),AddFriendSuccees);
    }
}

void Chat::receiveMessage()
{
    if(sender()==ChatWithFriendTcpSocket)
    {
        QString chatMessage,SendUserAccount;                //聊天记录和发送者的账号
        receiveTcpMessage(SendUserAccount,ChatWithFriendTcpSocket);
        receiveTcpMessage(chatMessage,ChatWithFriendTcpSocket);

        CreatChatView(SendUserAccount);                 //创建表
        db.open();
        QSqlQuery findChatMessage;
        findChatMessage.exec(tr("insert into %0(is_Judgeme,ChatText) values('false','%1')").arg(SendUserAccount,chatMessage));
        db.close();

        if(is_ChatPageShow)                 //界面显示的话
        {
            //判断是不是好友   是的话执行  这一堆  不是 的话执行另一堆
            if(SendUserAccount==ui->FriendAccountlabel->text().mid(3))
            {
                db.close();
                scrollArea->SetTextAndPicture(false,SendUserAccount,chatMessage);
                emit receiveChatMessage(SendUserAccount,chatMessage,false);
            }
            else
            {
                //BuildFilePathWriteChatMessage("界面打开但接受的消息并不是这个人的",SendUserAccount,NewChatMessage);
                emit receiveChatMessage(SendUserAccount,chatMessage,true);
            }
        }
        else                                //如果这个界面未显示的话
        {
            //BuildFilePathWriteChatMessage("界面打开但接受的消息并不是这个人的",SendUserAccount,NewChatMessage);
            qDebug()<<"发送emit";
            emit receiveChatMessage(SendUserAccount,chatMessage,true);
        }
    }
    else if(sender()==AddFriendTcpSocket)
    {
        QString Judge;
        receiveTcpMessage(Judge,AddFriendTcpSocket);
        if(Judge=="有用户")
        {
            ui->stackedWidget->setCurrentIndex(1);
            receiveNewFriend(AddFriendTcpSocket,newFriend);
            ui->AddFriendBt->show();
            qDebug()<<"有没有"<<newFriend.FriendCount;
            ui->Remarklabel->setText("无");
            ui->Nicknamelabel->setText("昵称："+newFriend.NickName);
            ui->FriendAccountlabel->setText("账号："+newFriend.FriendCount);
            ui->Arealabel->setText("地区："+newFriend.Area);


            QThread *thread=new QThread;                //申请新线程
            PictureThread *picturehread=new PictureThread(7,"HeadPicture",newFriend.FriendCount);     //创建线程对象
            qDebug()<<thread<<picturehread;                 //地址
            picturehread->moveToThread(thread);             //把对象移到线程
            connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(setSearchFriendPicture(QString)));         //绑定显示图片事件
            connect(this,SIGNAL(threadStart()),picturehread,SLOT(GOGOGO()));                            //绑定线程中的槽
            thread->start();                                            //线程运行
            emit threadStart();
        }
        else
        {
            LogInLabel=Judge;
            dialogloginJudge="ChatPage";           //提示界面判断
            DialogLogIn JudgeDialog(this);
            JudgeDialog.exec();
        }
    }
    else if(sender()==AddFriendSuccees)
    {

        Friend addsucceesFriend(newFriend.FriendCount,"无",newFriend.Age,newFriend.NickName,newFriend.Sex,newFriend.Area,true);
        emit  PushBackInFriendList(addsucceesFriend);           //pageme界面未接受
    }
}
void Chat::setSearchFriendPicture(QString fileName)
{
    ui->HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(fileName));
}

QDataStream& operator>>(QDataStream &in, newPeople &data)
{
    in>>data.FriendCount>>data.Age>>data.NickName>>data.Sex>>data.Area>>data.NoYesFriend;
    return in;
}

void Chat::receiveNewFriend(QTcpSocket *tcpSocket,newPeople &message)
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
    qDebug()<<message.FriendCount<<message.Area;

}

void Chat::on_SendMessage_clicked()
{
    SendChatMessage(ChatWithFriendTcpSocket);               //服务器不能启动新线程
    SendTcpMessage("11",ChatWithFriendTcpSocket);
    SendTcpMessage(UserAccount,ChatWithFriendTcpSocket);
    SendTcpMessage(ui->FriendAccountlabel->text().mid(3),ChatWithFriendTcpSocket);
    SendTcpMessage(ui->MessagelineEdit->text(),ChatWithFriendTcpSocket);

    db.open();
    QSqlQuery findChatMessage;
    findChatMessage.exec(tr("insert into %0(is_Judgeme,ChatText) values('true','%1')").arg(ui->FriendAccountlabel->text().mid(3),ui->MessagelineEdit->text()));
    db.close();
    scrollArea->SetTextAndPicture(true,UserAccount,ui->MessagelineEdit->text());
    emit receiveChatMessage(ui->FriendAccountlabel->text().mid(3),ui->MessagelineEdit->text(),false);           //给pageme发送账号和消息用以显示
    ui->MessagelineEdit->clear();
}


void Chat::on_SearchFriendAccount_textChanged(const QString &arg1)
{
    if(arg1=="")
    {
        ui->SearchFriendBt->hide();         //隐藏添加好友按钮
        ui->verticalLayout_3->setContentsMargins(0,0,0,heigth_Change(0.9)-width_Change(0.1));
    }
    else
    {
        ui->verticalLayout_3->setContentsMargins(0,0,0,heigth_Change(0.85)-width_Change(0.1));
        ui->SearchFriendBt->show();
        ui->SearchFriendBt->setText(tr("搜索：%0").arg(ui->SearchFriendAccount->text()));
    }
}

void Chat::on_SearchFriendBt_clicked()
{
   if(UserAccount==ui->SearchFriendAccount->text())
   {
       LogInLabel="添加的好友不能是自己";
       dialogloginJudge="ChatPage";           //提示界面判断
       DialogLogIn JudgeDialog(this);
       JudgeDialog.exec();
   }
   else
   {
        newConnect(AddFriendTcpSocket);
   }
}

void Chat::on_AddFriendBt_clicked()
{
    newConnect(AddFriendSuccees);

    ui->AddFriendBt->hide();
}
