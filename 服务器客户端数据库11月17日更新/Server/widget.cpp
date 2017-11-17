#include "widget.h"
#include "ui_widget.h"
#include<windows.h>
#include<QMessageBox>
#include<QBrush>
QMap<QString,OnlinePeople> OnlinePeopleMap;           //在线用户map
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ThreadCount=0;
    this->setWindowTitle("I Am FUWUQI!!!");
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



}
void Widget::addOnlineWidgetList(OnlinePeople online)
{
    OnlinePeopleMap.insert(online.UserAccount,online);
    ui->OnlinePeoplelistWidget->addItem(online.UserAccount);
}
void Widget::offPeople(QString UserAccount)
{
    QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(UserAccount);
    OnlinePeopleMap.erase(LookUp);
    ui->OnlinePeoplelistWidget->removeItemWidget(ui->OnlinePeoplelistWidget->takeItem(ui->OnlinePeoplelistWidget->row(ui->OnlinePeoplelistWidget->findItems(UserAccount,Qt::MatchExactly)[0])));
}


void Widget::monitor()
{
    mythread[ThreadCount]=new MyThread(tcpServer,ProcuctList,ThreadCount);
    qDebug()<<&mythread[ThreadCount];
    connect(mythread[ThreadCount],SIGNAL(OnlinePeopleAccount(OnlinePeople)),this,SLOT(addOnlineWidgetList(OnlinePeople)));
    connect(mythread[ThreadCount],SIGNAL(OffLinePeopleNumber(QString)),this,SLOT(offPeople(QString)));
    connect(mythread[ThreadCount],SIGNAL(finished()),mythread[ThreadCount],SLOT(quit()));
    //mythread[sss]->msleep(40);
    mythread[ThreadCount]->start();
    while(!mythread[ThreadCount]->isFinished());
    ThreadCount++;
    qDebug()<<"第几个：："<<ThreadCount;
    if(ThreadCount==10000)
        ThreadCount=0;
}


Widget::~Widget()
{
    delete ui;
    for(int i=0;i<ThreadCount;i++)
        delete mythread[ThreadCount];
    delete tcpServer;
    delete RecommendListModel;
    OnlinePeopleMap.clear();           //在线用户list
    ProcuctList.clear();
    qDebug()<<"删除啪啪啪";
}
/*****************************88推荐添加*********************/
void Widget::on_OkBt_clicked()
{
    if(ui->WritrEditLine->text().size()!=0)
    {
        this->adGoodsItem(ui->WritrEditLine->text());
        ui->WritrEditLine->clear();
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入商品编号！！！"), QMessageBox::Yes);
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


void Widget::on_DatabaseTableView_doubleClicked(const QModelIndex &index)
{
    if(index.column()!=0)
    {

        QMessageBox::warning(this, tr("警告"), tr("请点击添加第一列商品编号谢谢！！！"), QMessageBox::Yes);
        return;
    }
    else
    {
        this->adGoodsItem(ui->DatabaseTableView->currentIndex().data().toString());
    }

}
void Widget::adGoodsItem(const QString &data)
{
    db.open();
    QSqlQuery Choose;
    Choose.exec(tr("select * from FarmProductsMessage where CommodityCode='%0'").arg(data));
    if(Choose.next())
    {
        for(int i=0;i<ProcuctList.size();i++)
            if(Choose.value("CommodityCode").toString()==ProcuctList.at(i).CommodityCode)
            {
                QMessageBox::warning(this, tr("警告"), tr("请勿重复添加！！！"), QMessageBox::Yes);
                return;
            }
        RecommendListModel->setItem(ProcuctList.size(), 0, new QStandardItem(Choose.value("CommodityCode").toString()));
        RecommendListModel->setItem(ProcuctList.size(), 1, new QStandardItem(Choose.value("ProductName").toString()));
        RecommendListModel->setItem(ProcuctList.size(), 2, new QStandardItem("农产品"));
        Procuct s(Choose.value(0).toString(),Choose.value(1).toString(),Choose.value(2).toFloat(),Choose.value(3).toFloat(),Choose.value(4).toString(),Choose.value(5).toString(),Choose.value(6).toString(),Choose.value(7).toFloat(),Choose.value(8).toString(),Choose.value(9).toString());          //建商品对象准备加入链表
        ProcuctList.push_back(s);                               //加入链表
    }
    else
    {
        qDebug()<<"此物品以下架";
    }
}

void Widget::on_CompulsoryOffline_clicked()
{
    for(int i=0;i<NeedOffLine.size();i++)
    {
        qDebug()<<"asdqwdwqd"<<NeedOffLine[i];
        QMap<QString,OnlinePeople>::iterator LookUp=OnlinePeopleMap.find(NeedOffLine[i]);
        if(LookUp!=OnlinePeopleMap.end())
        {
            SendTcpMessage("您已被强制下线",LookUp.value().BackOnline());
            OnlinePeopleMap.erase(LookUp);
            ui->OnlinePeoplelistWidget->removeItemWidget(ui->OnlinePeoplelistWidget->takeItem(ui->OnlinePeoplelistWidget->row(ui->OnlinePeoplelistWidget->findItems(NeedOffLine[i],Qt::MatchExactly)[0])));
        }
    }
    NeedOffLine.clear();
}

void Widget::on_OnlinePeoplelistWidget_clicked(const QModelIndex &index)
{
    for(int i=0;i<NeedOffLine.size();i++)
        if(NeedOffLine[i]==index.data().toString())
        {
            QColor color("#ffffff");
            ui->OnlinePeoplelistWidget->item(index.row())->setBackgroundColor(color);
            NeedOffLine.removeAt(i);
            return;
        }
    QColor color(Qt::red);
    ui->OnlinePeoplelistWidget->item(index.row())->setBackgroundColor(color);
    NeedOffLine.push_back(index.data().toString());         //下线通知

}
void Widget::SendTcpMessage(QString message,QTcpSocket *tcpSocket_1)
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
