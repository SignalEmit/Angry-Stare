#include "productwindows.h"
#include "ui_productwindows.h"
#include<QPushButton>
#include<QDesktopWidget>
#include<QFileDialog>
#include<QMessageBox>
#include"interface.h"
extern int ProductWindowsPage;
extern QString UserAccount;         //引用账号
extern Procuct ProductMessagePage_FarmProduct;          //引用商品账号
extern Friend showFriend;                               //全局朋友，接收朋友的所有信息串回来
extern int ChatPage;

ProductWindows::ProductWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWindows)
{
    ui->setupUi(this);
    ui->scrollArea->setLayout(ui->gridLayout_5);
    this->setLayout(ui->verticalLayout);
    ui->page1->setLayout(ui->gridLayout);


    ui->gridLayout_3->setContentsMargins(0,0,0,heigth_Change(0.25));

    ui->page2->setLayout(ui->gridLayout_2);

    ui->page3->setLayout(ui->gridLayout_4);
    ui->gridLayout_4->setContentsMargins(0,0,0,heigth_Change(0.7));

    ui->page4->setLayout(ui->verticalLayout_Farm);


    ui->page5->setLayout(ui->verticalLayout_PartTimeJob);

    ui->FarmBack->setFixedWidth(width_Change(0.15));
    ui->PartTimeJobBack->setFixedWidth(width_Change(0.15));

    for(int i=1;i<=10000;i++)
        ui->comboBox_count->addItem(QString::number(i));
    ui->comboBox_count->setEditable(true);

    QStringList str;
    str<<"个"<<"斤"<<"千克"<<"吨";
    ui->comboBox_amount->addItems(str);

    ui->BackBt->setFixedSize(width_Change(0.1),width_Change(0.1));

    ui->gridLayout_5->setContentsMargins(0,0,0,heigth_Change(0.3));

    ui->FarmBack->setFixedSize(width_Change(0.1),width_Change(0.1));


    /*************************888按钮大小**************************/
    ui->FarmProductButton->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));            //商品

    ui->detailsBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));                     //详情

    ui->evaluationBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));             //评价


    ui->minusBt->setFixedSize(width_Change(0.08),width_Change(0.08));
    ui->addBt->setFixedSize(width_Change(0.08),width_Change(0.08));

    ui->ContactTheSellerBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));             //评价
    ui->BuyNowBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));             //评价
    ui->AddToCartBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));             //评价
    ui->AttentionBt->setIconSize(QSize(width_Change(0.15),width_Change(0.1)));             //评价

/**********************************8网络通信********************************/

    UpdataFarmProduct=new QTcpSocket(this);
    connect(UpdataFarmProduct, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(UpdataFarmProduct, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    ReceiveSeller=new QTcpSocket(this);
    connect(ReceiveSeller, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(ReceiveSeller, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    SendProductForDataBase=new QTcpSocket(this);
    connect(SendProductForDataBase, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(SendProductForDataBase, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    ui->FilePathlineEdit->setEnabled(false);

}
void ProductWindows::sendMessage()
{

    if(sender()==UpdataFarmProduct)
    {
        Procuct newFarmProduct("",ui->FarmProductNameLineEdit->text(),ui->CostLineEdit->text().toFloat(),ui->comboBox_count->lineEdit()->text().toInt(),ui->comboBox_amount->currentText(),
                               ui->ThingTextEdit->toPlainText(),"",0,UserAccount,ui->FarmPhoneProductlineEdit->text());                       //新的产品对 引号一商品编号，引号2图片位置  第三处0 是邮费  在下方判断
        if(ui->Yes_CostSender_radioBt->isChecked()  &&  ui->sendCostLineEdit->text().toFloat()>0)
            newFarmProduct.Postage=ui->sendCostLineEdit->text().toFloat();
        else
            qDebug()<<"此物品包邮";

        SendTcpMessage("15",UpdataFarmProduct);
        SendFarmProduct(newFarmProduct,UpdataFarmProduct);
    }
    else if(sender()==ReceiveSeller)
    {
        SendTcpMessage("19",ReceiveSeller);
        qDebug()<<"发送者账号："<<ProductMessagePage_FarmProduct.Promulgator;
        SendTcpMessage(ProductMessagePage_FarmProduct.Promulgator,ReceiveSeller);               //发送要接受的账号
    }
    else if(sender()==SendProductForDataBase)
    {
        SendTcpMessage("20",SendProductForDataBase);
    }

}

void ProductWindows::receiveMessage()
{
    if(sender()==UpdataFarmProduct)
    {
        QString ProductNumber;                              //产品编号
        receiveTcpMessage(ProductNumber,UpdataFarmProduct);
        qDebug()<<"产品编号:"<<ProductNumber;
        QThread *thread=new QThread;                //申请新线程
        PictureThread *picturehread=new PictureThread(14,PicTureType::FarmProduct,ui->FilePathlineEdit->text(),ProductNumber);     //创建线程对象
        picturehread->moveToThread(thread);             //把对象移到线程
        thread->start();

        ui->FilePathlineEdit->clear();
        ui->FarmProductNameLineEdit->text().clear();
        ui->CostLineEdit->text().clear();
        ui->comboBox_count->lineEdit()->text().clear();
        ui->ThingTextEdit->toPlainText().clear();
        ui->FarmPhoneProductlineEdit->text().clear();
        this->close();


    }
    else if(sender()==ReceiveSeller)
    {
        Friend newFriend;
        receiveTcpMessage_FriendList_Search(ReceiveSeller,newFriend);
        qDebug()<<"啪啪啪啪啪啪"<<newFriend.FriendCount<<newFriend.NoYesFriend<<newFriend.NickName<<newFriend.Remark;
        ChatPage=0;
        showFriend=newFriend;

        FriendChatPage.show();
    }
}

ProductWindows::~ProductWindows()
{
    delete ui;
}
void ProductWindows::showEvent(QShowEvent *e)
{
    qDebug()<<"发送者账号12312312："<<ProductMessagePage_FarmProduct.Promulgator;

    ui->amountLabel->setText("0");

    ui->stackedWidget->setCurrentIndex(ProductWindowsPage);
    if(ProductWindowsPage==0)
    {
        qDebug()<<"产品 的编号是"<<ProductMessagePage_FarmProduct.CommodityCode;
        ui->page->setStyleSheet(tr("border-image: url(./FarmProducts/%0)").arg(ProductMessagePage_FarmProduct.CommodityCode));
        ui->FarmProductNameLabel->setText("商品名："+ProductMessagePage_FarmProduct.ProductName);
        ui->FarmProductPriceLabel->setText("单价是："+QString::number(ProductMessagePage_FarmProduct.Cost));
        if(ProductMessagePage_FarmProduct.Postage==0)
            ui->is_FarmPoastLabel->setText("邮费：0元");
        else
            ui->is_FarmPoastLabel->setText("邮费："+QString::number(ProductMessagePage_FarmProduct.Postage));
    }
}
void ProductWindows::closeEvent(QCloseEvent *e)
{
    ProductWindowsPage=0;
}




void ProductWindows::on_FarmBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void ProductWindows::on_PartTimeJob_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void ProductWindows::on_SendPicture_clicked()
{
    QString a=QFileDialog::getOpenFileName(this, "选择文件", "/img", (";;jpg格式(*.jpg)"));
    ui->FilePathlineEdit->setText(a);
}


void ProductWindows::on_FarmProductOkBt_clicked()
{

    if(ui->FarmProductNameLineEdit->text().isEmpty()  ||  ui->FilePathlineEdit->text().isEmpty()  ||  ui->CostLineEdit->text().isEmpty()
             ||  ui->comboBox_count->lineEdit()->text().isEmpty()
             ||  ui->ThingTextEdit->toPlainText().isEmpty()  ||  ui->FarmPhoneProductlineEdit->text().isEmpty())
        QMessageBox::warning(this, tr("警告"), tr("所有东西均为必填"), QMessageBox::Yes);
    else
        newConnect(UpdataFarmProduct);
}

void ProductWindows::on_Yes_CostSender_radioBt_clicked()
{
    ui->sendCostLineEdit->setEnabled(true);
}

void ProductWindows::on_No_CostSender_radioBt_clicked()
{
    ui->sendCostLineEdit->clear();
    ui->sendCostLineEdit->setEnabled(false);
}

void ProductWindows::on_BackBt_clicked()
{
    this->close();
}

void ProductWindows::on_minusBt_clicked()
{
    int amount=ui->amountLabel->text().toInt();
    if(amount==0)
        return;
    else
        ui->amountLabel->setText(QString::number(--amount));
}

void ProductWindows::on_addBt_clicked()
{
    int amount=ui->amountLabel->text().toInt();
    ui->amountLabel->setText(QString::number(++amount));
}

void ProductWindows::on_ContactTheSellerBt_clicked()
{
    newConnect(ReceiveSeller);
}

void ProductWindows::on_BuyNowBt_clicked()
{
    if(ui->amountLabel->text().toInt()==0)
    {

        
        QMessageBox::warning(this, tr("警告"), tr("请输入购买数量"), QMessageBox::Yes);
        return;
    }
    ui->stackedWidget->setCurrentIndex(1);                  //跳转

    ui->ShowPictureLabel->setStyleSheet(tr("border-image:url(./FarmProducts/%0)").arg(ProductMessagePage_FarmProduct.CommodityCode));
    ui->ProductNameLabel->setText("商品名称："+ProductMessagePage_FarmProduct.ProductName);
    ui->ProductPieceLabel->setText(tr("单价：￥%0元/%1").arg(QString::number(ProductMessagePage_FarmProduct.Cost),ProductMessagePage_FarmProduct.MassUnit));
    ui->ProductNumberLabel->setText(tr("购买数量：%0%1").arg(ui->amountLabel->text(),ui->MassUnitComBox->currentText()));
    if(ProductMessagePage_FarmProduct.Postage==0)
            ui->ProductSendDeliverLabel->setText("此物品包邮");
    else
        ui->ProductSendDeliverLabel->setText(tr("此物品的邮费是：%0").arg(QString::number(ProductMessagePage_FarmProduct.Postage)));

    float coast;
    if(ProductMessagePage_FarmProduct.MassUnit==ui->MassUnitComBox->currentText())
        coast=ui->amountLabel->text().toInt()*ProductMessagePage_FarmProduct.Cost;
   // else if()
    ui->totalLabel->setText(tr("小计%0%1 共%2元").arg(ui->amountLabel->text(),ui->MassUnitComBox->currentText(),QString::number(coast)));
}
void ProductWindows::on_PlaceOrderBt_clicked()
{
    newConnect(SendProductForDataBase);
}
