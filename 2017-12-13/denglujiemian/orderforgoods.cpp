#include "orderforgoods.h"
#include "ui_orderforgoods.h"
#include"picturethread.h"
#include<QFile>
#include<QScroller>
extern QString UserAccount;
OrderForGoods::OrderForGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderForGoods)
{

    ui->setupUi(this);
    PayMessageList.clear();
    theListOfDeleteLayout.clear();
    is_FirstShow=true;
    ui->GoBackBt->setFixedSize(width_Change(0.1),width_Change(0.1));
    baseLayout=new QVBoxLayout;                    //底层排布
    base=new QWidget;
    base->setFixedSize(width_Change(0.9930),0);
    base->setLayout(baseLayout);


    ui->scrollArea->setWidget(base);

    ui->page->setLayout(ui->verticalLayout_page);
    this->setLayout(ui->verticalLayout);

    tcpSocket_buyGoodsMessage=new QTcpSocket(this);
    connect(tcpSocket_buyGoodsMessage, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_buyGoodsMessage, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    QScroller::grabGesture(ui->scrollArea,QScroller::TouchGesture);
    ReceiveOrderMessage=new ReceiveOrderTcpMessage(tcpSocket_buyGoodsMessage);
    connect(ReceiveOrderMessage,SIGNAL(SendTheList(QList<PayMessage>)),this,SLOT(ShowWholeGoods(QList<PayMessage>)));

    x_mousePress= 9990;                                 //获取下压的x坐标
    y_mousePress=9991;
    x_mouseRelease= 9992;                             //获取抬起的x坐标
    y_mouseRelease= 9993;
}
bool OrderForGoods::eventFilter(QObject *obj, QEvent *event)
{
    for(int i=0;i<theListOfDeleteLayout.size();i++)
        if(obj==theListOfDeleteLayout[i].BuyAgain)
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
                theListOfDeleteLayout[i].BuyAgain->setText("嬷嬷哒啪啪啪");

                y_mousePress=9991;                                             //防止进入消息循环
                return true;
            }
        }
    return false;
}

void OrderForGoods::showEvent(QShowEvent *e)
{
    if(is_FirstShow)
    {
        newConnect(tcpSocket_buyGoodsMessage);
        is_FirstShow=false;
    }
}
void OrderForGoods::sendMessage()
{
    if(sender()==tcpSocket_buyGoodsMessage)
    {
        SendTcpMessage("22",tcpSocket_buyGoodsMessage);
        SendTcpMessage(UserAccount,tcpSocket_buyGoodsMessage);
    }
}
void OrderForGoods::receiveMessage()
{
    if(sender()==tcpSocket_buyGoodsMessage)
    {
        qDebug()<<"接了吗";
        ReceiveOrderMessage->Receivelist();
    }


}
void OrderForGoods::ShowWholeGoods(QList<PayMessage>  _list)
{
    qDebug()<<"运行了吗";
    PayMessageList=_list;
    for(int i=0;i<PayMessageList.size();i++)
    {
        baseLayout->addWidget(SetGoods(PayMessageList[i].Promulgator,PayMessageList[i].Synopsis,tr("共%0%1商品 实付款：￥%2元  ").arg(PayMessageList[i].PayGoodsNumber).arg(PayMessageList[i].PayGoodsMassUnit).arg(QString::number(PayMessageList[i].PayGoodsMoney)) ,":/美工按钮专用图/图片未显示-01.png","再买一次"));
        base->setFixedSize(width_Change(0.9930),heigth_Change(0.30)*(i+1));
        QFile file("FarmProducts/"+PayMessageList.at(i).CommodityCode+".jpg");
        if(!file.exists())
        {
            QThread *thread=new QThread;                //申请新线程
            PictureThread *picturehread=new PictureThread(18,"FarmProducts",PayMessageList.at(i).CommodityCode);     //创建线程对象
            qDebug()<<thread<<picturehread;                 //地址
            picturehread->moveToThread(thread);             //把对象移到线程
            connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(SetGoodsPicture(QString)));         //绑定显示图片事件
            thread->start();
        }
        else
            theListOfDeleteLayout[i].GoodsPicture->setStyleSheet(tr("border-image: url(./FarmProducts/%0)").arg(PayMessageList.at(i).CommodityCode));
        qDebug()<<PayMessageList.size()<<"第"<<i<<"个";
    }
}
void OrderForGoods::SetGoodsPicture(QString fileNumber)
{
    for(int i=0;i<PayMessageList.size();i++)
        if(PayMessageList.at(i).CommodityCode==fileNumber)
        {
            theListOfDeleteLayout[i].GoodsPicture->setStyleSheet(tr("border-image: url(./FarmProducts/%0)").arg(PayMessageList.at(i).CommodityCode));
        }
}

QWidget* OrderForGoods::SetGoods(QString shopName,QString goodsMessage,QString payMessage,QString picturePath,QString btName)
{
    QGridLayout *GoodsMessage_Gridlayout;
    QLabel *PayMessage;
    QTextEdit *GoodsMessage;
    QLabel *GoodsPicture;
    QHBoxLayout *horizontalLayout;
    QLabel *ShopIcon;
    QLabel *ShopName;
    QSpacerItem *horizontalSpacer;
    QLabel *GoodsDelete;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *BuyAgain;
    QFrame *line_2;
    QFrame *line;
    QFrame *line_3;
    QWidget *baseWidget;

    GoodsMessage_Gridlayout = new QGridLayout;
    GoodsMessage_Gridlayout->setSpacing(6);
    GoodsMessage_Gridlayout->setContentsMargins(11, 11, 11, 11);
    PayMessage = new QLabel;
    PayMessage->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    GoodsMessage_Gridlayout->addWidget(PayMessage, 4, 0, 1, 4);
    GoodsMessage = new QTextEdit;
    GoodsMessage->setEnabled(false);
    GoodsMessage->setContextMenuPolicy(Qt::DefaultContextMenu);
    GoodsMessage->setAutoFillBackground(false);
    GoodsMessage->setStyleSheet(QLatin1String("border:none;\n"
    "border:1px solid red;color:black;"));
    GoodsMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    GoodsMessage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    GoodsMessage->setReadOnly(true);
    GoodsMessage->setOverwriteMode(true);
    GoodsMessage_Gridlayout->addWidget(GoodsMessage, 2, 1, 1, 3);
    GoodsPicture = new QLabel;
    GoodsPicture->setStyleSheet(QString::fromUtf8("border-image: url(%0);").arg(picturePath));
    GoodsMessage_Gridlayout->addWidget(GoodsPicture, 2, 0, 1, 1);
    horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing(6);
    ShopIcon = new QLabel;
    ShopIcon->setStyleSheet(QString::fromUtf8("border-image: url(:/美工按钮专用图/商品.png);"));
    horizontalLayout->addWidget(ShopIcon);
    ShopName = new QLabel(shopName+">");
    horizontalLayout->addWidget(ShopName);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    GoodsDelete = new QLabel;
    GoodsDelete->setStyleSheet(QString::fromUtf8("border-image: url(:/美工按钮专用图/删 除.png);"));
    horizontalLayout->addWidget(GoodsDelete);
    GoodsMessage_Gridlayout->addLayout(horizontalLayout, 0, 0, 1, 4);
    horizontalLayout_2 = new QHBoxLayout;
    horizontalLayout_2->setSpacing(6);
    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_3);
    BuyAgain = new QLabel(tr("%0 ").arg(btName));

    BuyAgain->setStyleSheet(QLatin1String("color:red;\n"
    "background-color:skyblue"));
    horizontalLayout_2->addWidget(BuyAgain);
    GoodsMessage_Gridlayout->addLayout(horizontalLayout_2, 6, 0, 1, 4);
    line_2 = new QFrame;
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    GoodsMessage_Gridlayout->addWidget(line_2, 3, 0, 1, 4);
    line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    GoodsMessage_Gridlayout->addWidget(line, 1, 0, 1, 4);
    line_3 = new QFrame;
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    GoodsMessage_Gridlayout->addWidget(line_3, 5, 0, 1, 4);

    GoodsMessage->setText(goodsMessage);
    PayMessage->setText(payMessage);
    baseWidget=new QWidget;
    baseWidget->setLayout(GoodsMessage_Gridlayout);
    baseWidget->setFixedSize(width_Change(0.9930),heigth_Change(0.275));


    ShopIcon->setFixedSize(width_Change(0.08),width_Change(0.08));
    GoodsDelete->setFixedSize(width_Change(0.08),width_Change(0.08));
    baseWidget->setStyleSheet("background-color:#f3fffd");


    BuyAgain->installEventFilter(this);                 //为不用again注册事件
    DeleteLayout theDeleteLayout(GoodsMessage_Gridlayout,PayMessage,GoodsMessage,GoodsPicture,horizontalLayout,ShopIcon,ShopName,horizontalSpacer,GoodsDelete,horizontalLayout_2,horizontalSpacer_3,BuyAgain,line_2,line,line_3,baseWidget);
    theListOfDeleteLayout.push_back(theDeleteLayout);
    return baseWidget;
}

void OrderForGoods::ShowThisPage(int PageNumber)
{
    ui->stackedWidget->setCurrentIndex(PageNumber);
}

OrderForGoods::~OrderForGoods()
{

    for(int i=0;i<theListOfDeleteLayout.size();i++)
    {
        delete theListOfDeleteLayout[i].BuyAgain;
        delete theListOfDeleteLayout[i].GoodsDelete;
        delete theListOfDeleteLayout[i].GoodsMessage;
        delete theListOfDeleteLayout[i].GoodsPicture;
        delete theListOfDeleteLayout[i].line;
        delete theListOfDeleteLayout[i].line_2;
        delete theListOfDeleteLayout[i].line_3;
        delete theListOfDeleteLayout[i].PayMessage;
        delete theListOfDeleteLayout[i].ShopIcon;
        delete theListOfDeleteLayout[i].ShopName;
        delete theListOfDeleteLayout[i].horizontalLayout_2;
        delete theListOfDeleteLayout[i].horizontalLayout;
        delete theListOfDeleteLayout[i].GoodsMessage_Gridlayout;
        delete theListOfDeleteLayout[i].baseWidget;
    }
    delete baseLayout;
    delete base;
    delete ui;

}

void OrderForGoods::on_Obligation_clicked()
{

}
