#include "MyScrollArea.h"
#include<QScroller>
#include<QDesktopWidget>
#include<QRect>
#include<QLabel>
#include<QDebug>
#include<QApplication>
ScrollArea::ScrollArea(QWidget *parent):QScrollArea(parent)
{
    this->setStyleSheet("QScrollBar:vertical {"
                        "border: none;"
                        "background-color: rgba(255, 255, 255);"
                        "width: 4px;"
                        "margin: 0px 0 0px 0;"
                    "}"
                    "QScrollBar::handle:vertical {"
                       "border: none;"
                       "border-radius:2px;"
                        "background: rgba(154, 154, 154, 150);"
                        "min-height: 15px;"
                    "}"
                  "QScrollBar::sub-line:vertical {"
                       "border: none;"
                        "height: 0px;"
                        "subcontrol-position: top;"
                        "subcontrol-origin: margin;"
                    "}"
                  "QScrollBar::add-line:vertical {"
                        "border: none;"
                        "height: 0px;"
                        "subcontrol-position: bottom;"
                        "subcontrol-origin: margin;"
                    "}"
                    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                      "border:none;"
                        "width: 0px;"
                        "height: 0px;"
                    "}"
                    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                        "background: none;"
                    "}");
    QDesktopWidget* desktopWidget = QApplication::desktop();                        //获取桌面
    QRect screenRect = desktopWidget->screenGeometry();
    sWidth = screenRect.width();
    sHeight=screenRect.height();
    base=new QWidget;                                                               //申请一片空白区
    base->setFixedWidth(width_Change(0.9930));
    BaseVerticalLayout=new QVBoxLayout;                                             //在空白区上设置一个排布器
    base->setLayout(BaseVerticalLayout);                                            //将排布器适应base区域
    this->setWidget(base);
    trueListItemNumber=0;                                                           //初始化，区域是否要申请
    QScroller::grabGesture(this,QScroller::TouchGesture);                           //滑动事件
}
void ScrollArea::InitializtionScrollArea()                                          //控件初始化0
{
    base->setFixedSize(width_Change(0.9930),0);
    trueListItemNumber=0;                   //初始化0
    for(int i=0;i<ItemList.size();i++)
     {
        ItemList[i].HeadPicture->hide();                                            //将屏幕上所有的控件隐藏 以便base的重新设置
        ItemList[i].Text->hide();
    }
}
void ScrollArea::SetTextAndPicture(bool is_me,const QString &PicturePath,const QString &Text)       //设置用户头像和用户聊天信息
{
    bool Judgebuttom=false;
    if(this->verticalScrollBar()->value()==this->verticalScrollBar()->maximum())
            Judgebuttom=true;
    trueListItemNumber++;                                                           //更新计算的这个数
    if(is_me)                                                                       //判断发送消息是否是本用户发出的
        pushMyMessage(PicturePath,Text);                                            //是的话设置头像和照片并显示
    else                                                                            //判断发送消息是否是朋友发出的
        pushFriendMessage(PicturePath,Text);
    if(Judgebuttom)
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}
void ScrollArea::pushFriendMessage(const QString &PicturePath,const QString &Text_)
{
    QHBoxLayout *HBoxLayout;
    QLabel *HeadPicture;
    QLabel *Text;

    if(trueListItemNumber>ItemList.size())                                          //如果需要的条目数量大于ItemList.size() 那么申请一个item
    {
        HBoxLayout=new QHBoxLayout;                                                 //申请头像，排布和消息显示标签
        HeadPicture=new QLabel;
        Text=new QLabel;
        HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PicturePath));
        HeadPicture->setFixedSize(width_Change(0.1),width_Change(0.1));
        Text->setText(Text_);
        Text->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        Text->setFixedHeight(width_Change(0.1));
        HBoxLayout->addWidget(HeadPicture);
        HBoxLayout->addWidget(Text);
        Item item(HeadPicture,Text,HBoxLayout);
        ItemList.push_back(item);
        BaseVerticalLayout->addLayout(HBoxLayout);
        base->setFixedHeight(width_Change(0.1)*ItemList.size());
        trueListItemNumber=ItemList.size();                                         //更新trueListItemNumber和ItemList.size()相等
    }
    else
    {
        qDebug()<<"爬爬爬爬爬摸摸大1";
        ItemList[trueListItemNumber-1].HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PicturePath));
        ItemList[trueListItemNumber-1].HeadPicture->setFixedSize(width_Change(0.1),width_Change(0.1));
        ItemList[trueListItemNumber-1].Text->setText(Text_);
        ItemList[trueListItemNumber-1].Text->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ItemList[trueListItemNumber-1].HBoxLayout->addWidget(ItemList[trueListItemNumber-1].HeadPicture);
        ItemList[trueListItemNumber-1].HBoxLayout->addWidget(ItemList[trueListItemNumber-1].Text);
        ItemList[trueListItemNumber-1].HeadPicture->show();
        ItemList[trueListItemNumber-1].Text->show();
        base->setFixedHeight(width_Change(0.1)*trueListItemNumber);
    }
}
void ScrollArea::pushMyMessage(const QString &PicturePath,const QString &Text_)
{
    QHBoxLayout *HBoxLayout;
    QLabel *HeadPicture;
    QLabel *Text;
    if(trueListItemNumber>ItemList.size())
    {
        HBoxLayout=new QHBoxLayout;
        HeadPicture=new QLabel;
        Text=new QLabel;
        HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PicturePath));
        HeadPicture->setFixedSize(width_Change(0.1),width_Change(0.1));
        Text->setText(Text_);
        Text->setFixedHeight(width_Change(0.1));
        Text->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        HBoxLayout->addWidget(Text);
        HBoxLayout->addWidget(HeadPicture);
        Item item(HeadPicture,Text,HBoxLayout);
        ItemList.push_back(item);
        BaseVerticalLayout->addLayout(HBoxLayout);
        base->setFixedHeight(width_Change(0.1)*ItemList.size());
        trueListItemNumber=ItemList.size();
    }
    else
    {
        qDebug()<<"爬爬爬爬爬摸摸大2";
        ItemList[trueListItemNumber-1].HeadPicture->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(PicturePath));
        ItemList[trueListItemNumber-1].HeadPicture->setFixedSize(width_Change(0.1),width_Change(0.1));
        ItemList[trueListItemNumber-1].Text->setText(Text_);
        ItemList[trueListItemNumber-1].Text->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ItemList[trueListItemNumber-1].HBoxLayout->addWidget(ItemList[trueListItemNumber-1].Text);
        ItemList[trueListItemNumber-1].HBoxLayout->addWidget(ItemList[trueListItemNumber-1].HeadPicture);
        ItemList[trueListItemNumber-1].HeadPicture->show();
        ItemList[trueListItemNumber-1].Text->show();
        base->setFixedHeight(width_Change(0.1)*trueListItemNumber);
    }
}

int ScrollArea::heigth_Change(float Percentage)
{
    return sHeight*Percentage;
}
int ScrollArea::width_Change(float Percentage)
{
    return sWidth*Percentage;
}
ScrollArea::~ScrollArea()
{
    delete base;
    for(int i=0;i<ItemList.size();i++)
    {
        delete ItemList[i].HBoxLayout;
        delete ItemList[i].HeadPicture;
        delete ItemList[i].Text;
    }
}
