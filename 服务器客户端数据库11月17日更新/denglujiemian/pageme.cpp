#include "pageme.h"
#include "ui_pageme.h"
#include"interface.h"
#include<QDesktopWidget>
#include<QDebug>
#include<QObjectList>
#include<QScroller>
extern int MessagePage;             //消息界面（其实是pageme界面）的页码数
int ChatPage;                       //交谈界面的页码数
extern QString UserAccount;         //引用账号
Friend showFriend;                  //需要传递的朋友账号
PageMe::PageMe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMe)
{
    ui->setupUi(this);
/*************************************************************************************************/
/*****************************************网络*****************************************************/
/*************************************************************************************************/
   // is_FristShowTheCount=true;                      //是否第一次显示初始化数组

    showAgain=true;

    tcpSocket_FriendList=new QTcpSocket(this);
    connect(tcpSocket_FriendList, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_FriendList, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_FriendList, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpSocket_ANewChatMessageFriend=new QTcpSocket(this);
    connect(tcpSocket_ANewChatMessageFriend, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_ANewChatMessageFriend, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_ANewChatMessageFriend, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));



 /***************************************888滑动区************************************************/



    int newsreel_Up=width_Change(0.1);
    ui->BackBt->setFixedSize(newsreel_Up,newsreel_Up);          //返回按钮设置大小
    ui->NewsBt->setFixedSize(newsreel_Up,newsreel_Up);          //返回按钮设置大小
    ui->label_2->setFixedHeight(newsreel_Up);



    this->setLayout(ui->verticalLayout);                //主界面绑定排布


    ui->label_2->setFixedWidth(width_Change(0.8));                          //设置标题名字把联系人和消息挤过去

     ui->label_3->setFixedHeight(4);


    /*********************************设置按钮大小**************************************/
    ui->SearchFriend->setFixedHeight(width_Change(0.125));
    ui->AddFriend->setFixedHeight(width_Change(0.125));

    /**********************************好友界面绑定*******************************/
     ui->FriendWidget->setFixedSize(width_Change(0.9930),2*width_Change(0.125)+10);
    ui->FriendWidget->setLayout(ui->verticalLayout_3);
    ui->FriendscrollArea->setWidget(ui->FriendWidget);
    ui->FriendPage->setLayout(ui->verticalLayout_2);

    /********************************************************************************/
    /********************************************************************************/
    /************************好友滑动区************************************************/
    /********************************************************************************/
    /********************************************************************************/
    QScroller::grabGesture(ui->FriendscrollArea, QScroller::TouchGesture);







    /************************************消息界面实例***********************/
    ui->NewsWidget->setLayout(ui->NewsVerticalLayout);
    ui->NewsPage->setLayout(ui->verticalLayout_4);
    ui->NewsScrollArea->setWidget(ui->NewsWidget);

    QScroller::grabGesture(ui->NewsScrollArea, QScroller::TouchGesture);
    x_mousePress=5000;                                                          //java中int默认0防止显示窗口
    y_mousePress=8000;

    connect(&man,SIGNAL(receiveChatMessage(QString,QString,bool)),this,SLOT(ChatMessageShowNewUser(QString,QString,bool)));      //监听这个端口
    connect(&man,SIGNAL(PushBackInFriendList(Friend)),this,SLOT(AddSuccessInList(Friend)));
    connect(this,SIGNAL(createChatSocket()),&man,SLOT(SetUpChatSocker()));
}
void PageMe::showEvent(QShowEvent *e)
{
    ui->stackedWidget->setCurrentIndex(MessagePage);
    if(MessagePage==1)
        ui->NewsBt->setStyleSheet(QLatin1String("border-image: url(:/ICON/sucai/config_set_128px_548300_easyicon.net.ico);"));
    else
        ui->NewsBt->setStyleSheet(QLatin1String("border-image: url(:/ICON/picture/xiaoxi.ico);"));
    if(showAgain)
        newConnect(tcpSocket_FriendList);

}

void PageMe::closeEvent(QCloseEvent *e)
{
    showAgain=false;
}
void PageMe::setChatPicture(QString PictureName)
{
    for(int i=0;i<ChatUserList.size();i++)
        if(ChatUserList.at(i)==PictureName)
        {
            Label1_Text[i]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PictureName));
            break;
        }
}
PageMe::~PageMe()
{
    FriendList.clear();
    delete tcpSocket_FriendList;
    delete ui;
    delete tcpSocket_ANewChatMessageFriend;
    ChatUserList.clear();
    FriendList.clear();

}
bool PageMe::eventFilter(QObject *obj, QEvent *event)
{
    for(int i=0;i<FriendList.size();i++)
        if (obj ==friendlabel[i])                                              //获取点击区指针
    {
        QMouseEvent *mouseEvent= static_cast<QMouseEvent *>(event);        //获取点击区指针
        if (event->type() == QEvent::MouseButtonPress)                      //如果等于下压事件
        {
            x_mousePress = mouseEvent->x();                                 //获取下压的x坐标
            y_mousePress = mouseEvent->y();                                         //获取下压的y坐标
        }
        if (event->type() == QEvent::MouseButtonRelease)                       //如果等于抬起事件
        {
            x_mouseRelease = mouseEvent->x();                               //获取抬起的x坐标
            y_mouseRelease = mouseEvent->y();                               //获取抬起的y坐标
        }
        if (x_mousePress == x_mouseRelease&&y_mousePress == y_mouseRelease) //当抬起和下压相等时发生点击事件
        {
           ChatPage=1;
           showFriend=FriendList.at(i);
           man.show();                                                 //显示
            y_mousePress=9635;                                             //防止进入消息循环
            return true;
         }
}

  for(int i=0;i<ChatUserList.size();i++)
    if (obj == Widget_Text[i])                                              //获取点击区指针
    {
        QMouseEvent *mouseEvent= static_cast<QMouseEvent *>(event);        //获取点击区指针
        if (event->type() == QEvent::MouseButtonPress)                      //如果等于下压事件
        {
            x_mousePress = mouseEvent->x();                                 //获取下压的x坐标
            y_mousePress = mouseEvent->y();                                         //获取下压的y坐标
        }
        if (event->type() == QEvent::MouseButtonRelease)                       //如果等于抬起事件
        {
            x_mouseRelease = mouseEvent->x();                               //获取抬起的x坐标
            y_mouseRelease = mouseEvent->y();                               //获取抬起的y坐标
        }
        if (x_mousePress == x_mouseRelease&&y_mousePress == y_mouseRelease) //当抬起和下压相等时发生点击事件
        {
            ChatPage=0;
            showFriend.FriendCount=ChatUserList.at(i);
            timestar[i].stop();

            man.show();                                           //显示

            y_mousePress=9632;                                             //防止进入消息循环
            return true;
         }

    }
  return false;
}


void PageMe::sendMessage()
{
    if(sender()==tcpSocket_FriendList)
    {
        SendTcpMessage("6",tcpSocket_FriendList);
        SendTcpMessage(UserAccount,tcpSocket_FriendList);
        qDebug()<<"发送的账号是"<<UserAccount;
    }
    else if(sender()==tcpSocket_ANewChatMessageFriend)
    {
        SendTcpMessage("14",tcpSocket_ANewChatMessageFriend);
    }
}

void PageMe::receiveMessage()
{
    if(sender()==tcpSocket_FriendList)
    {
        QString FriendCount;        //朋友个数
        receiveTcpMessage(FriendCount,tcpSocket_FriendList);
        for(int i=0;i<FriendCount.toInt();i++)
        {
            Friend ANewFriend;
            receiveTcpMessage_FriendList_Search(tcpSocket_FriendList,ANewFriend);
            FriendList.push_back(ANewFriend);
        }


        for(int i=0;i<FriendList.size();i++)
        {
            friendlabel[i]=new QLabel(tr("%0").arg(FriendList.at(i).NickName));
            friendpicturelabel[i]=new QLabel();
            friendHBoxLayout[i]=new QHBoxLayout;
            friendHBoxLayout[i]->addWidget(friendpicturelabel[i]);
            friendHBoxLayout[i]->addWidget(friendlabel[i]);
            friendpicturelabel[i]->setFixedSize(width_Change(0.25),width_Change(0.15));



            ui->FriendWidget->setFixedSize(width_Change(0.9930),(2+i)*width_Change(0.15)+4+50*(i+1));
            ui->verticalLayout_3->addLayout(friendHBoxLayout[i]);

            friendpicturelabel[i]->setFixedHeight(width_Change(0.15));


            friendlabel[i]->installEventFilter(this);

        }
        QFile FriendPictureSize;
        for(int i=0;i<FriendList.size();i++)
        {
            FriendPictureSize.setFileName("HeadPicture/"+FriendList.at(i).FriendCount+".jpg");
            //qDebug()<<"发送这位同学的照片："+FriendList.at(i).FriendCount<<FriendList.at(i).FriendPictureSize<<QString::number(FriendPictureSize.size());
            if(!FriendPictureSize.exists() || FriendList.at(i).FriendPictureSize!=QString::number(FriendPictureSize.size()))
            {
                QThread *thread=new QThread;                //申请新线程
                PictureThread *picturehread=new PictureThread(7,"HeadPicture",FriendList.at(i).FriendCount);     //创建线程对象
                qDebug()<<thread<<picturehread;                 //地址
                picturehread->moveToThread(thread);             //把对象移到线程
                connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(SetFriendPicture(QString)));         //绑定显示图片事件
                thread->start();
            }//线程运行
            else
            {
                friendpicturelabel[i]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(FriendList.at(i).FriendCount));
            }
        }
    }
}
void PageMe::SetFriendPicture(QString PictureName)
{
    for(int i=0;i<FriendList.size();i++)
        if(FriendList.at(i).FriendCount==PictureName)
        {
            friendpicturelabel[i]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PictureName));
            break;
        }
}

void PageMe::displayError(QAbstractSocket::SocketError)
{

}
/*****************************************88网络***********************************/
/*
QDataStream& operator>>(QDataStream &in, Friend &data)
{
    in>>data.FriendCount>>data.Remark>>data.Age>>data.NickName>>data.Sex>>data.Area>>data.NoYesFriend;
    return in;
}
void PageMe::receiveTcpMessage_FriendList(QTcpSocket *tcpSocket,Friend &message)
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
*/
void PageMe::TimeStar()
{
    for (int i = 0; i < ChatUserList.size(); i++)
    {
        if (sender() == &timestar[i])
        {
            Widget_Text[i]->setStyleSheet("background-color: rgb(85, 255, 255);");
            break;
        }
    }
}
void PageMe::TimeEnd()
{
    for (int i = 0; i < ChatUserList.size(); i++)
    {
        if (sender() == &timeend[i])
        {
            Widget_Text[i]->setStyleSheet("background-color: #f0f0f0;");
            break;
        }
    }
}
void PageMe::ChatMessageShowNewUser(QString ChatMessageFriendAccount,QString ChatMessage,bool is_Shine)
{

    if(ChatUserList.size()==0)
        ChatMessagePageAddNews(ChatMessageFriendAccount,ChatMessage,is_Shine);
    else
    {
        for(int i=0;i<ChatUserList.size();i++)
            if(ChatUserList.at(i)==ChatMessageFriendAccount)
            {
                Label4_Text[i]->setText(ChatMessage);
                Label2_Text[i]->setText(ChatMessageFriendAccount);
                if(is_Shine)
                {
                    connect(&timestar[i], SIGNAL(timeout()), this, SLOT(TimeStar()));
                    connect(&timeend[i], SIGNAL(timeout()), this, SLOT(TimeEnd()));
                    timestar[i].start(1000);
                    timeend[i].start(2001);
                }
                break;
            }
            else
            {
                if(i==ChatUserList.size()-1)
                    ChatMessagePageAddNews(ChatMessageFriendAccount,ChatMessage,is_Shine);
            }
    }

}
void PageMe::ChatMessagePageAddNews(QString ChatMessageFriendAccount,QString ChatMessage,bool is_Shine)
{
    ChatUserList.push_back(ChatMessageFriendAccount);           //链表长度加1
    Widget_Text[ChatUserList.size()-1]=new QWidget;
    Widget_Text[ChatUserList.size()-1]->setFixedSize(width_Change(0.9930),300);                 //申请一个300的空间

    GridLayout_Text[ChatUserList.size()-1]=new QGridLayout;
    Label1_Text[ChatUserList.size()-1]=new QLabel;
    Label1_Text[ChatUserList.size()-1]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(ChatMessageFriendAccount));
    Label2_Text[ChatUserList.size()-1]=new QLabel(ChatMessageFriendAccount);

    Label3_Text[ChatUserList.size()-1]=new QLabel;                      //这个待定

    Label4_Text[ChatUserList.size()-1]=new QLabel(ChatMessage);

    GridLayout_Text[ChatUserList.size()-1]->addWidget(Label1_Text[ChatUserList.size()-1],0,0,3,1);
    GridLayout_Text[ChatUserList.size()-1]->addWidget(Label2_Text[ChatUserList.size()-1],0,3,1,1);
    GridLayout_Text[ChatUserList.size()-1]->addWidget(Label3_Text[ChatUserList.size()-1],1,3,1,1);
    GridLayout_Text[ChatUserList.size()-1]->addWidget(Label4_Text[ChatUserList.size()-1],2,3,1,1);
    Widget_Text[ChatUserList.size()-1]->setLayout(GridLayout_Text[ChatUserList.size()-1]);

    ui->NewsWidget->setFixedSize(width_Change(0.9930),325*(ChatUserList.size()));           //设置界面大小实例的适合要改变
    Label1_Text[ChatUserList.size()-1]->setFixedHeight(300);
    ui->NewsVerticalLayout->addWidget(Widget_Text[ChatUserList.size()-1]);
    Widget_Text[ChatUserList.size()-1]->installEventFilter(this);                               //注册事件

    QFile file(tr("HeadPicture/%0.jpg").arg(ChatMessageFriendAccount));
    if(!file.exists())
    {
        QThread *thread=new QThread;                //申请新线程
        PictureThread *picturehread=new PictureThread(7,"HeadPicture",ChatMessageFriendAccount);     //创建线程对象
        qDebug()<<thread<<picturehread;                 //地址
        picturehread->moveToThread(thread);             //把对象移到线程
        connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(setChatPicture(QString)));         //绑定显示图片事件

        thread->start();//线程运行
    }

    if(is_Shine)
    {
        connect(&timestar[ChatUserList.size()-1], SIGNAL(timeout()), this, SLOT(TimeStar()));
        connect(&timeend[ChatUserList.size()-1], SIGNAL(timeout()), this, SLOT(TimeEnd()));
        timestar[ChatUserList.size()-1].start(1000);
        timeend[ChatUserList.size()-1].start(2001);
     }

}

void PageMe::AddSuccessInList(Friend newfriend)
{
    qDebug()<<"街道信号";
    FriendList.push_back(newfriend);
    friendlabel[FriendList.size()-1]=new QLabel(tr("%0").arg(newfriend.NickName));
    friendpicturelabel[FriendList.size()-1]=new QLabel();
    friendHBoxLayout[FriendList.size()-1]=new QHBoxLayout;
    friendHBoxLayout[FriendList.size()-1]->addWidget(friendpicturelabel[FriendList.size()-1]);
    friendHBoxLayout[FriendList.size()-1]->addWidget(friendlabel[FriendList.size()-1]);
    friendpicturelabel[FriendList.size()-1]->setFixedSize(width_Change(0.25),width_Change(0.15));
    // friendpicturelabel[i]->setStyleSheet("background-color:blue");


    ui->FriendWidget->setFixedSize(width_Change(0.9930),(1+FriendList.size())*width_Change(0.15)+4+50*(FriendList.size()));
    ui->verticalLayout_3->addLayout(friendHBoxLayout[FriendList.size()-1]);

    friendpicturelabel[FriendList.size()-1]->setFixedHeight(width_Change(0.15));
    friendpicturelabel[FriendList.size()-1]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(newfriend.FriendCount));
    //friendlabel[i]->setStyleSheet("background-color:red");
    friendlabel[FriendList.size()-1]->installEventFilter(this);
    /*
    for(int i=0;i<FriendList.size();i++)
    {
        if(FriendList.at(i).FriendCount==newfriend.FriendCount)
            break;
        else if(i==FriendList.size()-1)
        {
            qDebug()<<"街道信号";
            FriendList.push_back(newfriend);
            friendlabel[FriendList.size()-1]=new QLabel(tr("%0").arg(newfriend.NickName));
            friendpicturelabel[FriendList.size()-1]=new QLabel();
            friendHBoxLayout[FriendList.size()-1]=new QHBoxLayout;
            friendHBoxLayout[FriendList.size()-1]->addWidget(friendpicturelabel[FriendList.size()-1]);
            friendHBoxLayout[FriendList.size()-1]->addWidget(friendlabel[FriendList.size()-1]);
            friendpicturelabel[FriendList.size()-1]->setFixedSize(width_Change(0.25),width_Change(0.15));
            // friendpicturelabel[i]->setStyleSheet("background-color:blue");


            ui->FriendWidget->setFixedSize(width_Change(0.9930),(1+FriendList.size())*width_Change(0.15)+4+50*(FriendList.size()));
            ui->verticalLayout_3->addLayout(friendHBoxLayout[FriendList.size()-1]);

            friendpicturelabel[FriendList.size()-1]->setFixedHeight(width_Change(0.15));
            friendpicturelabel[FriendList.size()-1]->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(newfriend.FriendCount));
            //friendlabel[i]->setStyleSheet("background-color:red");
            friendlabel[FriendList.size()-1]->installEventFilter(this);
        }
    }
    */
}

/*******************************各种槽******************************/
void PageMe::on_BackBt_clicked()
{
    this->close();
}

void PageMe::on_NewsBt_clicked()
{
    ui->NewsBt->setStyleSheet(QLatin1String("border-image: url(:/ICON/sucai/config_set_128px_548300_easyicon.net.ico);"));
    ui->stackedWidget->setCurrentIndex(1);
}

void PageMe::on_AddFriend_clicked()
{
    ChatPage=2;

    man.show();
}
