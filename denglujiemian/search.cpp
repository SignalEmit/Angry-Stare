#include "search.h"
#include "ui_search.h"
#include<QMessageBox>
#include<QMouseEvent>
extern int ProductWindowsPage;                  //引用页面页数全局变量
extern Procuct ProductMessagePage_FarmProduct;              //
Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    is_FristShowTheCount=true;                      //是否第一次显示初始化数组
/*****************************数据库****************************************/
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("MyDateBase.db");




    /***********************查询为空*******************************/
    SearchEmpty=new QLabel("未查询到任何东西");
    SearchEmpty->setFixedHeight(heigth_Change(0.1));
    SearchEmpty->setAlignment(Qt::AlignCenter);
    /***********************查询为空*******************************/



    /**************************初始化鼠标下压建***************************************/
    x_mousePress= 9990;                                 //获取下压的x坐标
    y_mousePress=9991;
    x_mouseRelease= 9992;                             //获取抬起的x坐标
    y_mouseRelease= 9993;
     /**************************初始化鼠标下压建***************************************/




    UpdataNumber=0;                 //控件更新到0

    press_y[0]= 0;
    move_y[0]= -1;
    release_y[0] = 0;
    animation[0] = new QPropertyAnimation();

    press_y[1]= 0;
    move_y[1]= -1;
    release_y[1] = 0;
    animation[1] = new QPropertyAnimation();

    press_y[2]= 0;
    move_y[2]= -1;
    release_y[2] = 0;
    animation[2] = new QPropertyAnimation();


    this->setLayout(ui->gridLayout);

    ui->page->setLayout(ui->verticalLayout);
    ui->page_2->setLayout(ui->verticalLayout_2);
    ui->page_3->setLayout(ui->gridLayout_2);

/******************************8商品查询显示界面********************************/
    gridLayout=new QGridLayout;
    widget=new QWidget;
    widget->setLayout(gridLayout);

    ui->scrollArea->setWidget(widget);
/******************************8商品查询显示界面********************************/

    /**********************************调整按钮大小************************************/

    ui->BackBt->setFixedSize(width_Change(0.1),width_Change(0.1));
    ui->SearchBt->setFixedSize(width_Change(0.1),width_Change(0.1));
    ui->ClearListWidget->setFixedHeight(width_Change(0.11));
    ui->ClearListWidget->setIconSize(QSize(width_Change(0.1),width_Change(0.1)));
    ui->SearchLineEdit->setFixedHeight(width_Change(0.1));

    /**********************************调整按钮大小************************************/

    m_scrollBarV[0]=ui->listWidget->verticalScrollBar();
    QObjectList objectList = ui->listWidget->children();
    for(int i = 0; i < objectList.count(); i++)
    {
        objectList.at(i)->setObjectName(QStringLiteral("qt_scrollarea_viewport"));
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport")
        {
            objectList.at(i)->installEventFilter(this);
        }
    }

    m_scrollBarV[1]=ui->SearchListWidget->verticalScrollBar();
    QObjectList objectList1 = ui->SearchListWidget->children();
    for(int i = 0; i < objectList1.count(); i++)
    {
        objectList1.at(i)->setObjectName(QStringLiteral("momoda"));
        if(objectList1.at(i)->objectName() == "momoda")
        {
            objectList1.at(i)->installEventFilter(this);
        }
    }

    m_scrollBarV[2]=ui->scrollArea->verticalScrollBar();
    QObjectList objectList2 = ui->scrollArea->children();
    for(int i = 0; i < objectList2.count(); i++)
    {
        objectList2.at(i)->setObjectName(QStringLiteral("scroll"));
        if(objectList2.at(i)->objectName() == "scroll")
        {
            objectList2.at(i)->installEventFilter(this);
        }
    }


    /********************************网络**************************************/
    SearchMessage=new QTcpSocket(this);                                            //查找匹配的商品名称
    connect(SearchMessage, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(SearchMessage, SIGNAL(readyRead()), this, SLOT(receiveMessage()));


    ReceiveProductMessage=new QTcpSocket(this);                  //接收简单的商品信息
    connect(ReceiveProductMessage, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(ReceiveProductMessage, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
}

bool Search::eventFilter(QObject *obj, QEvent *event)
{

    for(int i=0;i<ProductList.size();i++)
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
              ProductWindowsPage=0;                       //访问农产品详情页
              ProductMessagePage_FarmProduct=ProductList.at(i);


              FarmProductPage.show();                                                 //显示

              y_mousePress=9991;                                             //防止进入消息循环
              return true;
           }

      }












    if(obj->objectName()!="scroll" && obj->objectName()!="momoda"  && obj->objectName()!="qt_scrollarea_viewport" )
       return false;
    else if("momoda"==obj->objectName())
        return SlidingRegin(event,obj,m_scrollBarV[1],animation[1],pressDateTime[1],press_y[1],move_y[1],release_y[1]);
    else if("scroll"==obj->objectName())
        return SlidingRegin(event,obj,m_scrollBarV[2],animation[2],pressDateTime[2],press_y[2],move_y[2],release_y[2]);
    else if("qt_scrollarea_viewport" == obj->objectName())
         return SlidingRegin(event,obj,m_scrollBarV[0],animation[0],pressDateTime[0],press_y[0],move_y[0],release_y[0]);
}
void Search::sendMessage()
{
    if(sender()==SearchMessage)
    {
        SendTcpMessage("16",SearchMessage);
        SendTcpMessage(ui->SearchLineEdit->text(),SearchMessage);
    }
    else if(sender()==ReceiveProductMessage)
    {
        SendTcpMessage("17",ReceiveProductMessage);
        SendTcpMessage(ReceiveNeedShowProductMessage,ReceiveProductMessage);
    }
}

void Search::receiveMessage()
{
    if(sender()==SearchMessage)
     {

        QString ProductNumber;              //产品个数
        receiveTcpMessage(ProductNumber,SearchMessage);
        for(int i=0;i<ProductNumber.toInt();i++)
        {
            QString FarmProductNumber;                                              //各个产品名称
            receiveTcpMessage(FarmProductNumber,SearchMessage);                     //就仅仅拿了个编号 什么鬼GG
            ui->SearchListWidget->addItem(FarmProductNumber);
        }
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(sender()==ReceiveProductMessage)
    {
        SearchEmpty->hide();                                //每次接到查找事件时首先  把  查找 为标签隐藏

        QString ProductNumber;
        receiveTcpMessage(ProductNumber,ReceiveProductMessage);
        if(ProductNumber.toInt()!=0)
        {
            for(int i=0;i<ProductNumber.toInt();i++)
            {
                Procuct newProduct;
                receiveTcpMessage_ListFarmPruduct(ReceiveProductMessage,newProduct);
                ProductList.push_back(newProduct);                        //入商品链表
                qDebug()<<newProduct.CommodityCode;
                UpdataSearchProductView();
            }
        }
        else
        {
            gridLayout->addWidget(SearchEmpty);
            widget->setFixedSize(width_Change(0.9930),heigth_Change(0.11));
            SearchEmpty->show();
        }

    }
}
void Search::UpdataSearchProductView()                        //产品界面的更新操作
{

    for(int i=0;i<ProductList.size();i++)
    {
       if(i>=UpdataNumber)
       {
            Widget_Text[i]=new QWidget;
            GridLayout_Text[i]=new QGridLayout;
            Label1_Text[i]=new QLabel;
            Label2_Text[i]=new QLabel;
            Label3_Text[i]=new QLabel;
            Label4_Text[i]=new QLabel;
            UpdataNumber++;
       }

        Label2_Text[i]->setText(ProductList.at(i).ProductName);
        Label3_Text[i]->setText(ProductList.at(i).Synopsis);
        Label4_Text[i]->setText("￥"+QString::number(ProductList.at(i).Cost)+"元/"+ProductList.at(i).MassUnit);

        widget->setFixedSize(width_Change(0.9930),300*(i+1)+35);
        Widget_Text[i]->setFixedSize(width_Change(0.9930),300);
        Label1_Text[i]->setFixedSize(width_Change(0.9930)/2,300);
        GridLayout_Text[i]->addWidget(Label1_Text[i],0,0,3,1);
        GridLayout_Text[i]->addWidget(Label2_Text[i],0,3,1,1);
        GridLayout_Text[i]->addWidget(Label3_Text[i],1,3,1,1);
        GridLayout_Text[i]->addWidget(Label4_Text[i],2,3,1,1);
        Widget_Text[i]->setLayout(GridLayout_Text[i]);
        Widget_Text[i]->show();
        gridLayout->addWidget(Widget_Text[i],4+i,0,1,5);
        Widget_Text[i]->installEventFilter(this);                               //注册事件
    }
    ReceiveProductPicture();
}
void Search::ReceiveProductPicture()
{
    for(int i=0;i<ProductList.size();i++)
    {
        QThread *thread=new QThread;                //申请新线程
        PictureThread *picturehread=new PictureThread(18,"FarmProducts",ProductList.at(i).CommodityCode);     //创建线程对象
        picturehread->moveToThread(thread);             //把对象移到线程
        connect(picturehread,SIGNAL(SendFileName(QString)),this,SLOT(SetRecommendPicture(QString))); //绑定显示图片事件
        thread->start();                                            //线程运行
    }
}
void Search::SetRecommendPicture(QString PictureCommodityCode)
{
    for(int i=0;i<ProductList.size();i++)
    {
        if(PictureCommodityCode==ProductList.at(i).CommodityCode)
        {

            Label1_Text[i]->setStyleSheet(tr("border-image: url(./FarmProducts/%0)").arg(PictureCommodityCode));
            break;
        }
    }
}

Search::~Search()
{
    delete ui;
}

void Search::on_SearchLineEdit_textChanged(const QString &arg1)
{
    if(ui->stackedWidget->currentIndex()==0||ui->stackedWidget->currentIndex()==1)
    {
        if(ui->SearchLineEdit->text().isEmpty())
            ui->stackedWidget->setCurrentIndex(0);
        else
        {
            ui->SearchListWidget->clear();                      //清空以前显示的
            newConnect(SearchMessage);
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
    else
    {
        qDebug()<<"啥都不做";
    }
}

void Search::on_listWidget_itemClicked(QListWidgetItem *item)                       //历史记录界面查询商品
{
    for(int i=0;i<ProductList.size();i++)
        Widget_Text[i]->hide();

    ProductList.clear();
    ReceiveNeedShowProductMessage=item->text();             //给需要搜索的东西赋值

    for(int i=0;i<ui->listWidget->count();i++)                                      //遍历整个listwidget
        if(ui->listWidget->item(i)->text()==ReceiveNeedShowProductMessage)          //比对
        {
            ui->listWidget->removeItemWidget(ui->listWidget->item(i));              //有相同的了删除
            break;
        }
    ui->listWidget->insertItem(0,ReceiveNeedShowProductMessage);                    //添加

    newConnect(ReceiveProductMessage);
    ui->stackedWidget->setCurrentIndex(2);


    ui->SearchLineEdit->setText(item->text());                          //给文本框一个Text


    OperatDataBaseHistory(QString("insert into SearchHistory(History) values('%0')").arg(ReceiveNeedShowProductMessage));
}



void Search::on_SearchListWidget_itemClicked(QListWidgetItem *item)                     //物品查找界面插叙商品
{
    for(int i=0;i<ProductList.size();i++)
        Widget_Text[i]->hide();

    ProductList.clear();

    ReceiveNeedShowProductMessage=item->text();                                     //给需要搜索的东西赋值

    for(int i=0;i<ui->listWidget->count();i++)                                      //遍历整个listwidget
        if(ui->listWidget->item(i)->text()==ReceiveNeedShowProductMessage)          //比对
        {
            ui->listWidget->removeItemWidget(ui->listWidget->item(i));              //有相同的了删除
            break;
        }
    ui->listWidget->insertItem(0,ReceiveNeedShowProductMessage);                    //添加


    newConnect(ReceiveProductMessage);
    ui->stackedWidget->setCurrentIndex(2);


    ui->SearchLineEdit->setText(item->text());                          //给文本框一个Text

    OperatDataBaseHistory(QString("insert into SearchHistory(History) values('%0')").arg(ReceiveNeedShowProductMessage));

}

void Search::on_ClearListWidget_clicked()
{
    ui->listWidget->clear();                            //清空列表
    OperatDataBaseHistory("delete  from SearchHistory");
}
void Search::OperatDataBaseHistory(QString str)
{
    db.open();
    QSqlQuery DeleteDataBase;
    if(DeleteDataBase.exec(str))
    {
        qDebug()<<"成功";
    }
    else
    {
      QSqlQuery InsertDataBase;
      InsertDataBase.exec(tr("delete from SearchHistory where History='%0'").arg(ReceiveNeedShowProductMessage));           //删除原有的值
      InsertDataBase.exec(QString("insert into SearchHistory(History) values('%0')").arg(ReceiveNeedShowProductMessage));       //插一条值插到最后
    }
    db.close();
}
void Search::showEvent(QShowEvent *e)
{
    db.open();
    QSqlQuery SearchDataBase;                           //查值
    SearchDataBase.exec("select * from SearchHistory order by rowid desc");                 //查询所有值
    while(SearchDataBase.next())
    {

        ui->listWidget->addItem(SearchDataBase.value("History").toString());
    }
    db.close();

    if(is_FristShowTheCount)                             //如果是第一次显示
    {
        Label1_Text=new QLabel*[50];
        Label2_Text=new QLabel*[50];
        Label3_Text=new QLabel*[50];
        Label4_Text=new QLabel*[50];
        Widget_Text=new QWidget*[50];
        GridLayout_Text=new QGridLayout*[50];
        is_FristShowTheCount=false;                      //第一次显示过了已经初始化
    }

}

void Search::closeEvent(QCloseEvent *e)
{
    ui->listWidget->clear();
}

void Search::on_SearchBt_clicked()
{
    if(!ui->SearchLineEdit->text().isEmpty())
    {
        for(int i=0;i<ProductList.size();i++)
            Widget_Text[i]->hide();

        ProductList.clear();
        ReceiveNeedShowProductMessage=ui->SearchLineEdit->text();             //给需要搜索的东西赋值

        for(int i=0;i<ui->listWidget->count();i++)                                      //遍历整个listwidget
            if(ui->listWidget->item(i)->text()==ReceiveNeedShowProductMessage)          //比对
            {
                ui->listWidget->removeItemWidget(ui->listWidget->item(i));              //有相同的了删除
                break;
            }
        ui->listWidget->insertItem(0,ReceiveNeedShowProductMessage);                    //添加


        newConnect(ReceiveProductMessage);
        ui->stackedWidget->setCurrentIndex(2);

        OperatDataBaseHistory(QString("insert into SearchHistory(History) values('%0')").arg(ReceiveNeedShowProductMessage));

    }
    else
        QMessageBox::warning(this, tr("警告"), tr("不能搜索空白的"), QMessageBox::Yes);
}

void Search::on_BackBt_clicked()
{
    this->close();
}
