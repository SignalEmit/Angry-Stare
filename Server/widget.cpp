#include "widget.h"
#include "ui_widget.h"
#include<windows.h>
QList<ChatUser> ChatUserList;                   //聊天界面套接字
QList<OnlinePeople> OnlinePeopleList;           //在线用户list
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    sss=0;
    /*************************建立农产品文件夹********************************/
    QDir filepath;//新建文件夹
    filepath.mkdir("FarmProducts");
    filepath.mkdir("HeadPicture");


    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::AnyIPv4, 666))
    {
        qDebug() << tcpServer->errorString();
        this->close();
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(monitor()));


    /***************数据库****************/
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("Message.db");


    RecommendListModel=new QStandardItemModel;              //表格显示
    RecommendListModel->setHorizontalHeaderItem(0, new QStandardItem("单号"));   //表头
    RecommendListModel->setHorizontalHeaderItem(1, new QStandardItem("品名"));//表头
    RecommendListModel->setHorizontalHeaderItem(2, new QStandardItem("简介"));//表头
    ui->tableView->setModel(RecommendListModel);

    this->setLayout(ui->gridLayout_3);
    ui->page->setLayout(ui->gridLayout);
    ui->page_2->setLayout(ui->gridLayout_2);

    ui->page_3->setLayout(ui->gridLayout_4);

}
void Widget::addOnlineWidgetList(OnlinePeople online)
{
    qDebug()<<"猜猜这是谁"<<online.UserAccount<<OnlinePeopleList.size();
    OnlinePeopleList.push_back(online);
    qDebug()<<"猜猜这是谁"<<online.UserAccount<<OnlinePeopleList.size();
    ui->OnlinePeoplelistWidget->addItem(online.UserAccount);
}
void Widget::offPeople(int UserNumber)
{
    qDebug()<<"退出"<<OnlinePeopleList.size();
    OnlinePeopleList.removeAt(UserNumber);
    ui->OnlinePeoplelistWidget->removeItemWidget(ui->OnlinePeoplelistWidget->takeItem(UserNumber));
}


void Widget::monitor()
{
    mythread[sss]=new MyThread(tcpServer,ProcuctList,sss);
    qDebug()<<&mythread[sss];
    connect(mythread[sss],SIGNAL(OnlinePeopleAccount(OnlinePeople)),this,SLOT(addOnlineWidgetList(OnlinePeople)));
    connect(mythread[sss],SIGNAL(OffLinePeopleNumber(int)),this,SLOT(offPeople(int)));
    connect(mythread[sss],SIGNAL(finished()),mythread[sss],SLOT(quit()));
    mythread[sss]->start();
    mythread[sss]->msleep(50);
    sss++;
    qDebug()<<"第几个：："<<sss;
    if(sss==10000)
        sss=0;





}


Widget::~Widget()
{
    delete ui;
    for(int i=0;i<sss;i++)
        delete mythread[sss];
    delete tcpServer;
    delete RecommendListModel;
    OnlinePeopleList.clear();           //在线用户list
    ProcuctList.clear();
    qDebug()<<"删除啪啪啪";
}
/*****************************88推荐添加*********************/
void Widget::on_OkBt_clicked()
{
    if(ui->FarmRadioButton->isChecked())
    {
        qDebug()<<"嬷嬷哒";
        db.open();
        QSqlQuery Choose;
        Choose.exec(tr("select * from FarmProductsMessage where CommodityCode='%0'").arg(ui->WritrEditLine->text()));
        db.close();
        ui->WritrEditLine->clear();
        if(Choose.next())
        {
            for(int i=0;i<ProcuctList.size();i++)           //遍历全部链表防止重复添加
            {
                QModelIndex idex = RecommendListModel->index(i, 0);
                if(Choose.value(0)==RecommendListModel->data(idex).toString())
                {
                    qDebug()<<"不要重复添加";
                    return;
                }
            }
            RecommendListModel->setItem(ProcuctList.size(), 0, new QStandardItem(Choose.value(0).toString()));
            RecommendListModel->setItem(ProcuctList.size(), 1, new QStandardItem(Choose.value(1).toString()));
            RecommendListModel->setItem(ProcuctList.size(), 2, new QStandardItem("农产品"));

            qDebug()<<Choose.value(0).toString()<<Choose.value(1).toString()<<Choose.value(2).toFloat()<<Choose.value(3).toInt()<<Choose.value(4).toString()<<Choose.value(5).toString()<<Choose.value(6).toString()<<Choose.value(7).toFloat()<<Choose.value(8).toString()<<Choose.value(9).toString();
            Procuct s(Choose.value(0).toString(),Choose.value(1).toString(),Choose.value(2).toFloat(),Choose.value(3).toInt(),Choose.value(4).toString(),Choose.value(5).toString(),Choose.value(6).toString(),Choose.value(7).toFloat(),Choose.value(8).toString(),Choose.value(9).toString());          //建商品对象准备加入链表
            ProcuctList.push_back(s);                               //加入链表

        }
        else
        {
            qDebug()<<"不存在";
        }
    }
    else if(ui->PartTimeJobradioButton->isChecked())
    {

    }
}
/****************************删除推荐******************************/
void Widget::on_DeleteBt_clicked()
{
    int row = ui->tableView->currentIndex().row();
    RecommendListModel->removeRow(row);
    for(int i=0;i<ProcuctList.size();i++)
    {
        if(row==i)
        {
            ProcuctList.removeAt(i);
        }
    }

    qDebug()<<ProcuctList.size();
}

void Widget::on_pushButton_clicked()
{

}

void Widget::on_RecommendPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_OnlinePeople_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_dateLookUp_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);              //跳转到数据查询
}

void Widget::on_UpdateDateBase_clicked()
{
    if(ui->LookUpFarmProduct->isChecked())
    {
        qDebug()<<"啊是的我去额";
        //实例QSqlTableModel
        QSqlQueryModel *model=new QSqlQueryModel;
        db.open();
        QSqlQuery lookUp("select CommodityCode 商品编号,ProductName 商品名,Cost 价格,Number 数量,MassUnit 单价,Synopsis 简介,PictureLocation 图片路径,Postage 邮费,Promulgator 发布者,ContactNumber 发布者手机号 from FarmProductsMessage");
        model->setQuery(lookUp);
        db.close();

        ui->DatabaseTableView-> setModel (model);
        db.close();
        //表格只读，默认单元格可修改
        ui->DatabaseTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    }
}

void Widget::on_AddTheList_clicked()
{
    if(ui->DatabaseTableView->currentIndex().column()!=0)
    {

        qDebug()<<"请选择第一列编号谢谢";
        return;
    }
    else
    {
        db.open();
        QSqlQuery Choose;
        Choose.exec(tr("select * from FarmProductsMessage where CommodityCode='%0'").arg(ui->DatabaseTableView->currentIndex().data().toString()));
        if(Choose.next())
        {
            for(int i=0;i<ProcuctList.size();i++)
                if(Choose.value("CommodityCode").toString()==ProcuctList.at(i).CommodityCode)
                {
                    qDebug()<<"请勿重复添加嬷嬷哒";
                    return;
                }
            RecommendListModel->setItem(ProcuctList.size(), 0, new QStandardItem(Choose.value("CommodityCode").toString()));
            RecommendListModel->setItem(ProcuctList.size(), 1, new QStandardItem(Choose.value("ProductName").toString()));
            RecommendListModel->setItem(ProcuctList.size(), 2, new QStandardItem("农产品"));
            Procuct s(Choose.value(0).toString(),Choose.value(1).toString(),Choose.value(2).toFloat(),Choose.value(3).toInt(),Choose.value(4).toString(),Choose.value(5).toString(),Choose.value(6).toString(),Choose.value(7).toFloat(),Choose.value(8).toString(),Choose.value(9).toString());          //建商品对象准备加入链表
            ProcuctList.push_back(s);                               //加入链表
        }
        else
        {
            qDebug()<<"此物品以下架";
        }
    }


}
