#include "interface.h"
#include "ui_interface.h"
#include<QDesktopWidget>
#include<QPixmap>
#include<QPushButton>
#include<QDebug>
#include<QWidget>
#include<QGridLayout>
#include<QFileDialog>
#include<QScroller>
#include"widget.h"
int MessagePage;                    //消息页码页码数
int ProductWindowsPage;             //商品界面信息（用于显示供求界面）
extern QString dialogloginJudge;           //提示界面判断
extern QString UserAccount;         //引用账号
Procuct ProductMessagePage_FarmProduct;
Interface::Interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Interface)
{
    ui->setupUi(this);

    is_FristShowTheCount=true;                      //是否第一次显示初始化数组
    //this->setWindowFlags(Qt::FramelessWindowHint);

    /*********************************8实例图片改变的按钮***********************/
    PictureChangeCount=0;                   //计数器初值为0
    for(int i=0;i<7;i++)
    {
        ChangePageLabel[i]=new QLabel;
        ChangePageLabel[i]->setStyleSheet("background-color:#e2e2e2");
        ui->horizontalLayout_8->addWidget(ChangePageLabel[i]);
    }
    ChangePageLabel[0]->setStyleSheet("background-color: rgb(0, 255, 255)");
    ui->horizontalLayout_8->setContentsMargins(width_Change(0.55),360,0,3);


    connect(&ChangePicture,SIGNAL(timeout()),this,SLOT(PictureChangeSlot()));       //定时器绑定事件
    ChangePicture.start(2500);                              //定时器启动






    SendRecommendPictureCount=0;                //接收推荐和发送推荐赋初值0
    /************************网络********************************/
    /************************网络********************************/
    /************************网络********************************/
    /************************网络********************************/



    tcpSocket_Recommend=new QTcpSocket(this);
    connect(tcpSocket_Recommend, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_Recommend, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_Recommend, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));


    Online=new QTcpSocket(this);
    connect(Online, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(Online, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(Online, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    Offline=new QTcpSocket(this);
    connect(Offline, SIGNAL(connected()), this, SLOT(sendMessage()));
     connect(Offline, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

     tcpSocket_CheckFileSize=new QTcpSocket(this);
     connect(tcpSocket_CheckFileSize, SIGNAL(connected()), this, SLOT(sendMessage()));
     connect(tcpSocket_CheckFileSize, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

 /************************网络********************************/
 /************************网络********************************/

    this->setLayout(ui->verticalLayout);

    //*****************************************************//
    //*****************************************************//
    //*********************设置logo和搜索框*******************//
    //*****************************************************//
    //*****************************************************//
    int newsreel_Up=width_Change(0.1);
    newsreel_Button=width_Change(0.15);

    ui->Other->setFixedSize(newsreel_Up,newsreel_Up);
    ui->Logo->setFixedSize(newsreel_Up,newsreel_Up);
    /*
    ui->homePageBt->setFixedSize(newsreel,newsreel);
    ui->MeBt->setFixedSize(newsreel,newsreel);
    ui->recommendBt->setFixedSize(newsreel,newsreel);
     */

    ui->homePageBt->setIconSize(QSize(width_Change(0.13),width_Change(0.13)));
    ui->MeBt->setIconSize(QSize(width_Change(0.13),width_Change(0.13)));
    ui->recommendBt->setIconSize(QSize(width_Change(0.13),width_Change(0.13)));



    ui->SearchBt->setFixedHeight(newsreel_Up);


    ui->ChangHeadStackWidget->setFixedHeight(newsreel_Up);
    ui->SearchHead->setLayout(ui->horizontalLayout_7);
    ui->LabelStackWidget->setLayout(ui->verticalLayout_7);




//*****************设置搜索******************************//
    QIcon icon(":/pic/search_query_128px_1063401_easyicon.net.ico");
    ui->SearchBt->setIcon(icon);
    ui->SearchBt->setIconSize(QSize(newsreel_Up, newsreel_Up));


    //*****************************************************//
    //*****************************************************//
    //*********************第一页的显示框*******************//
    //*****************************************************//
    //*****************************************************//


    ScrollAreaPage1= new QScrollArea;                                       //新建区域1
    ui->verticalLayout_5->addWidget(ScrollAreaPage1);                       //将滑块区加入排布
    ScrollAreaPage1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);     //设置滚动条永久可见
    ScrollAreaPage1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ScrollAreaPage1->setLayout(ui->gridLayout);                             //将待显示东西放入滑块区
    ui->page->setLayout(ui->verticalLayout_5);                              //将滑块区放去第一页


    ui->widget_3->setGeometry(0,0,width_Change(0.9930),newsreel_Button*2+50+heigth_Change(0.2)+75);               //设置滑动区大小

    ui->widget_3->setLayout(ui->gridLayout);                                //将排布放去等待放入区
    ScrollAreaPage1->setWidget(ui->widget_3);                               //滑动区放置大小


    ui->label_2->setFixedHeight(50);


    //*****************************************************//
    //*****************************************************//
    //*********************第二页的显示框*******************//
    //**********此页待定预留以后添加新功能使用****************************//
    //*****************************************************//
    ScrollAreaPage2=new QScrollArea;
    ScrollAreaPage2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);     //设置滚动条永久可见

    ui->SclloAreaPage_2verticalLayout->addWidget(ScrollAreaPage2);
    ui->Page2_Widget->setGeometry(0,0,width_Change(0.9930),3000);
   // ui->Page2_Widget->setLayout(ui->verticalLayout_6);                //此处添加

    ScrollAreaPage2->setWidget(ui->Page2_Widget);
    ui->page_2->setLayout(ui->SclloAreaPage_2verticalLayout);




    //*****************************************************//
    //*****************************************************//
    //*********************第三页的显示框*******************//
    //*****************************************************//
    //*****************************************************//

    ui->page_3->setLayout(ui->horizontalLayout_3);
    ui->page_4->setLayout(ui->gridLayout_2);
    ui->verticalLayout_6->setContentsMargins(0,0,0,heigth_Change(0.5));
    ui->gridLayout_2->setContentsMargins(0,0,0,heigth_Change(0.45));



    /********************************************设置界面************************************/
    ui->page_8->setLayout(ui->gridLayout_4);
    /********************************************************************************/
    /********************************************************************************/
    /************************功能区设置大小********************************************/
    /********************************************************************************/
    /********************************************************************************/

    ui->pushButton_3->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_2->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_4->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_5->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_6->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_7->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_10->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_11->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_12->setFixedSize(newsreel_Button,newsreel_Button);
    ui->pushButton_13->setFixedSize(newsreel_Button,newsreel_Button);
    /********************************************************************************/
    /********************************************************************************/
    /************************滑动区***************************************************/
    /********************************************************************************/
    /********************************************************************************/
    QScroller::grabGesture(ScrollAreaPage1, QScroller::TouchGesture);
    QScroller::grabGesture(ScrollAreaPage2, QScroller::TouchGesture);
    /********************************************************************************/
    /********************************************************************************/
    /**********************************推荐商品**********************************/
    /********************************************************************************/
    /********************************************************************************/

    x_mousePress= 9990;                                 //获取下压的x坐标
    y_mousePress=9991;
    x_mouseRelease= 9992;                             //获取抬起的x坐标
    y_mouseRelease= 9993;




    ui->widget_Stackedwidget->setFixedHeight(heigth_Change(0.2));
    ui->stackedWidget_3->installEventFilter(this);

    ui->widget_Stackedwidget->setLayout(ui->verticalLayout_8);

   /*****************************************888接收完推荐链表后显示界面然后接受图片********************/
    connect(this,SIGNAL(ShowPicture()),this,SLOT(ReceivePictureAndShow()));
    connect(this,SIGNAL(PageMePageShowChatMessage(QString,QString,bool)),&pageme,SLOT(ChatMessageShowNewUser(QString,QString,bool)));
    connect(this,SIGNAL(SendChatViewSocket()),&pageme,SLOT(activeChatSocket()));        //创建聊天套接字
    connect(this,SIGNAL(ChangeSettingPage(int)),&setting,SLOT(SetPage(int)));
    connect(&setting,SIGNAL(SetHeadPicture(QString)),this,SLOT(SetHeadPictureIcon(QString)));
}
void Interface::PictureChangeSlot()
{
    PictureChangeCount++;
    if(PictureChangeCount==7)
        PictureChangeCount=0;

    for(int i=0;i<7;i++)
        if(i==PictureChangeCount)
        {
         //  ChangePageLabel[i]->setStyleSheet("background-color: rgb(0, 255, 255);");
           ui->stackedWidget_3->setCurrentIndex(i);
        }
       // else
          // ChangePageLabel[i]->setStyleSheet("background-color: #e2e2e2;");
}

void Interface::closeEvent(QCloseEvent *e)
{
    e->ignore();//阻塞关闭事件
    connect(&dialog_close,SIGNAL(ExitProduct()),this,SLOT(offLine()));
    dialog_close.exec();                                                    //拟态关闭对话框
}

void Interface::ChangePage(int pageNumber)
{
        ui->stackedWidget->setCurrentIndex(pageNumber);                //当在主界面点击时更换页面
}



bool Interface::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->stackedWidget_3->widget(0) || obj == ui->stackedWidget_3->widget(1) ||obj == ui->stackedWidget_3->widget(2) ||obj == ui->stackedWidget_3->widget(3) ||obj == ui->stackedWidget_3->widget(4) ||obj == ui->stackedWidget_3->widget(5) ||obj == ui->stackedWidget_3->widget(6))                                             //获取点击区指针
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);        //获取点击区指针
        if (event->type() == QEvent::MouseButtonPress)                      //如果等于下压事件
        {
            x_mousePress = mouseEvent->x();                                 //获取下压的x坐标
            y_mousePress = mouseEvent->y();                                         //获取下压的y坐标
            return true;
        }
        if (event->type() == QEvent::MouseButtonRelease)                       //如果等于抬起事件
        {
            x_mouseRelease = mouseEvent->x();                               //获取抬起的x坐标
            y_mouseRelease = mouseEvent->y();                               //获取抬起的y坐标
            return true;
        }
        if (x_mousePress - x_mouseRelease>0	&& x_mousePress!=9990	&&	x_mouseRelease!=9992	&&	y_mouseRelease!=9993	&&	y_mousePress!=9991	&&	x_mouseRelease!= x_mousePress	&&	y_mousePress!= y_mouseRelease) //当抬起和下压相等时发生点击事件
        {
            if (ui->stackedWidget_3->currentIndex() == ui->stackedWidget_3->count() - 1)
            {
                PictureChangeCount=0;
                ui->stackedWidget_3->setCurrentIndex(0);

            }
            else
            {
                PictureChangeCount=ui->stackedWidget_3->currentIndex()+ 1;
                ui->stackedWidget_3->setCurrentIndex(ui->stackedWidget_3->currentIndex()+ 1);
            }

            ChangePicture.start(2500);                  //重启定时器

            x_mousePress = 9990;                                 //获取下压的x坐标
            y_mousePress = 9991;
            x_mouseRelease = 9992;                             //获取抬起的x坐标
            y_mouseRelease = 9993;
            return true;
        }
        else if (x_mousePress - x_mouseRelease<0 && x_mousePress != 9990 && x_mouseRelease != 9992 && y_mouseRelease != 9993 && y_mousePress != 9991 && x_mouseRelease != x_mousePress	&&	y_mousePress != y_mouseRelease)
        {
            if (ui->stackedWidget_3->currentIndex() == 0)
            {
                PictureChangeCount=ui->stackedWidget_3->count() - 1;
                ui->stackedWidget_3->setCurrentIndex(ui->stackedWidget_3->count() - 1);
            }
            else
            {
                PictureChangeCount=ui->stackedWidget_3->currentIndex() - 1;
                ui->stackedWidget_3->setCurrentIndex(ui->stackedWidget_3->currentIndex() - 1);
            }

            ChangePicture.start(2500);                  //重启定时器

            x_mousePress = 9990;                                 //获取下压的x坐标
            y_mousePress = 9991;
            x_mouseRelease = 9992;                             //获取抬起的x坐标
            y_mouseRelease = 9993;
            return true;
        }
        /*
        else
        {
            //ui.stackedWidget->widget(i)->setStyleSheet("background-color:red");

            return true;
        }
*/
        return false;
    }






  for(int i=0;i<GoodsWidgetList.size();i++)
    if (obj == GoodsWidgetList[i].BaseMap)                                              //获取点击区指针
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
            ProductWindowsPage=0;                       //访问农产品详情页
            ProductMessagePage_FarmProduct=RecommendProcuctList.at(i);


            FarmProductPage.show();                                                 //显示

            y_mousePress=9991;                                             //防止进入消息循环
            return true;
         }

    }
    return false;
}
Interface::~Interface()
{
    for(int i=0;i<GoodsWidgetList.size();i++)
    {
        delete GoodsWidgetList[i].BaseMap;
        delete GoodsWidgetList[i].GoodsMuch;
        delete GoodsWidgetList[i].GoodsName;
        delete GoodsWidgetList[i].GoodsPicture;
        delete GoodsWidgetList[i].GoodsSynopsis;
        delete GoodsWidgetList[i].MapGridLayout;
    }
    delete ui;
    delete tcpSocket_Recommend;
    delete Online;
    delete Offline;
    delete ScrollAreaPage1;
    delete ScrollAreaPage2;



    RecommendProcuctList.clear();
 //   for(int i=0;i<7;i++)
   //     delete ChangePageLabel[i];
}

void Interface::on_Other_clicked()
{
    MessagePage=1;
    pageme.show();
}




/************************网络********************************/
/************************网络********************************/
/************************网络********************************/
/************************网络********************************/
void Interface::offLine()
{
    newConnect(Offline);
}

void Interface::showEvent(QShowEvent *event)
{
   newConnect(tcpSocket_Recommend);
   newConnect(Online);

   if(is_FristShowTheCount)                             //如果是第一次显示
   {
       is_FristShowTheCount=false;                      //第一次显示过了已经初始化
   }
}

void Interface::displayError(QAbstractSocket::SocketError)
{

}
void Interface::receiveMessage()
{
    if(sender()==tcpSocket_Recommend)
    {
        QString ListCount;
        receiveTcpMessage(ListCount,tcpSocket_Recommend);
        for(int i=0;i<ListCount.toInt();i++)
        {
            Procuct ANewFarmProduct;
            receiveTcpMessage_ListFarmPruduct(tcpSocket_Recommend,ANewFarmProduct);
            RecommendProcuctList.push_back(ANewFarmProduct);
        }

        for(int i=0;i<RecommendProcuctList.size();i++)
        {
            ui->widget_3->setGeometry(0,0,width_Change(0.9930),newsreel_Button*2+heigth_Change(0.16)*(i+1)+100+heigth_Change(0.2)+25);
            QWidget *BaseMap=new QWidget;                                       //这组控件的底部
            BaseMap->setFixedSize(width_Change(0.9930),heigth_Change(0.15625));
            QGridLayout *MapGridLayout=new QGridLayout;                         //网格布局
            QLabel *GoodsPicture=new QLabel;                                    //商品照片
            GoodsPicture->setFixedSize(width_Change(0.9930)/2,heigth_Change(0.15625));
            QLabel *GoodsName=new QLabel(RecommendProcuctList.at(i).ProductName);//商品名
            QLabel *GoodsSynopsis=new QLabel;                                   //商品简介   下一个商品价额
            QLabel *GoodsMuch=new QLabel("￥"+QString::number(RecommendProcuctList.at(i).Cost)+"元/"+RecommendProcuctList.at(i).MassUnit);
            MapGridLayout->addWidget(GoodsPicture,0,0,3,1);
            MapGridLayout->addWidget(GoodsName,0,3,1,1);
            MapGridLayout->addWidget(GoodsSynopsis,1,3,1,1);
            MapGridLayout->addWidget(GoodsMuch,2,3,1,1);
            BaseMap->setLayout(MapGridLayout);
            BaseMap->show();
            ui->gridLayout->addWidget(BaseMap,4+i,0,1,5);
            GoodsSynopsis->setText(setLabelText(RecommendProcuctList.at(i).Synopsis,GoodsSynopsis->width()));
            BaseMap->installEventFilter(this);                               //注册事件
            GoodsWidget goods(BaseMap,MapGridLayout,GoodsPicture,GoodsName,GoodsSynopsis,GoodsMuch);
            GoodsWidgetList.push_back(goods);
        }
       newConnect(tcpSocket_CheckFileSize);
    }
    else if(sender()==Offline)
    {
        exit(0);
    }
    else if(sender()==Online)
    {
        QString Judge;
        receiveTcpMessage(Judge,Online);
        if(Judge=="异地登录")
        {
            qDebug()<<"异地登录";
            dialogloginJudge="您的账号以异地登录";
            dialogloginpage.exec();             //*&&&*//
        }
        else if(Judge=="您已被强制下线")
        {
            dialogloginJudge="您已被强制下线";
            dialogloginpage.exec();             //*&&&*//
        }
        else
        {
            emit SendChatViewSocket();
        }
    }
    else if(sender()==tcpSocket_CheckFileSize)
    {
        for(int i=0;i<RecommendProcuctList.size();i++)
        {
            QString RecommendProcuctSize;           //推荐文件大小
            receiveTcpMessage(RecommendProcuctSize,tcpSocket_CheckFileSize);
            RecommendProcuctList[i].PictureSize=RecommendProcuctSize;
        }
        emit ShowPicture();
    }
}

void Interface::sendMessage()
{
    if(sender()==tcpSocket_Recommend)
        SendTcpMessage("4",tcpSocket_Recommend);
    else if(sender()==Online)
    {
        SendTcpMessage("8",Online);
        SendTcpMessage(UserAccount,Online);
    }
    else if(sender()==Offline)
    {
        SendTcpMessage("9",Offline);
        SendTcpMessage(UserAccount,Offline);
    }
    else if(sender()==tcpSocket_CheckFileSize)
    {
        SendTcpMessage("20",tcpSocket_CheckFileSize);
        SendTcpMessage("Product",tcpSocket_CheckFileSize);
        SendTcpMessage(QString::number(RecommendProcuctList.size()),tcpSocket_CheckFileSize);           //发送数据大小
        for(int i=0;i<RecommendProcuctList.size();i++)
            SendTcpMessage(RecommendProcuctList.at(i).CommodityCode,tcpSocket_CheckFileSize);
    }
}
void Interface::ReceivePictureAndShow()
{
    QFile pictureFile;
    /****************************************88接收推荐列表照片*********************************/
    for(int i=0;i<RecommendProcuctList.size();i++)
    {
        pictureFile.setFileName("FarmProducts/"+RecommendProcuctList.at(i).CommodityCode+".jpg");
        if(!pictureFile.exists() || RecommendProcuctList.at(i).PictureSize!=QString::number(pictureFile.size()))
        {
            QThread *thread=new QThread;                //申请新线程
            PictureThread *picturehread=new PictureThread(5,"FarmProducts",QString::number(i));     //创建线程对象
            picturehread->moveToThread(thread);             //把对象移到线程
            connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(SetRecommendPicture(QString))); //绑定显示图片事件
            thread->start();
        }//线程运行
        else
        {
            GoodsWidgetList[i].GoodsPicture->setStyleSheet(tr("border-image: url(./%0)").arg(RecommendProcuctList.at(i).PictureLocation));
        }
    }
/****************************接收我的头像*********************************************/
    QThread *thread=new QThread;                //申请新线程
    PictureThread *picturehread=new PictureThread(7,"HeadPicture",UserAccount);     //创建线程对象
    qDebug()<<thread<<picturehread;                 //地址
    picturehread->moveToThread(thread);             //把对象移到线程
    connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(SetUserHeadPicture(QString)));         //绑定显示图片事件
    thread->start();                                            //线程运行

}
void Interface::SetUserHeadPicture(QString HeadPictureName)
{
    QFile::rename("HeadPicture/默认头像.jpg",tr("HeadPicture/%0").arg(HeadPictureName));
    QIcon icon(tr("HeadPicture/%0").arg(HeadPictureName));
    ui->SetUserAccount->setIcon(icon);
    ui->SetUserAccount->setIconSize(QSize(200,200));
    ui->SetUserAccount->setText(tr("%0             账号管理>>").arg(HeadPictureName));
}

void Interface::SetRecommendPicture(QString number)
{
    GoodsWidgetList[number.toInt()].GoodsPicture->setStyleSheet(tr("border-image: url(./%0)").arg(RecommendProcuctList.at(number.toInt()).PictureLocation));
}
void Interface::SetHeadPictureIcon(QString filepath)
{
    qDebug()<<"Filepath"<<filepath;
    QIcon icon(filepath);
    ui->SetUserAccount->setIcon(icon);
}

void Interface::on_SearchBt_clicked()
{
    Search *search=new Search();
    search->show();
}

/******************************88各种按钮***********************/
void Interface::on_MeBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->ChangHeadStackWidget->setCurrentIndex(1);
}

void Interface::on_homePageBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->ChangHeadStackWidget->setCurrentIndex(0);
}

void Interface::on_recommendBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->ChangHeadStackWidget->setCurrentIndex(0);
}

void Interface::on_Logo_clicked()
{
    MessagePage=0;
    pageme.show();
}

void Interface::on_PostYourWant_clicked()
{
    ProductWindowsPage=3;                           //发布消息
    ProductWindows *showProductWindows=new ProductWindows;                      //显示发布供求关系界面
    showProductWindows->show();
}

void Interface::on_SetUserAccount_clicked()
{
    emit ChangeSettingPage(0);
    setting.show();
}

void Interface::on_stackedWidget_3_currentChanged(int arg1)
{
    for(int i=0;i<7;i++)
        if(i==arg1)
        {

            ui->stackedWidget_3->widget(i)->setLayout(ui->horizontalLayout_8);
            ChangePageLabel[i]->setStyleSheet("background-color: rgb(0, 255, 255)");
        }
        else
            ChangePageLabel[i]->setStyleSheet("background-color:#e2e2e2");
}


void Interface::on_AboutAS_clicked()
{
    emit ChangeSettingPage(1);
    setting.show();
}
