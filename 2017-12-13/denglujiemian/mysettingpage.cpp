#include "mysettingpage.h"
#include "ui_mysettingpage.h"
#include<QFileDialog>
#include<QCloseEvent>
#include<QMessageBox>
extern QString UserAccount;
MySettingPage::MySettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySettingPage)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);                            //主界面自适应
    ui->page_1->setLayout(ui->verticalLayout_2);                    //界面一自适应
    ui->page_2->setLayout(ui->verticalLayout_3);
    ui->CloseBt->setFixedSize(width_Change(0.1),width_Change(0.1));

    ui->Age_label->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->Sex_label->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->Name_label->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->Nickname_label->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->AreaLocation_label->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));

    ui->Age_lineEdit->setFixedHeight(heigth_Change(0.06));
    ui->AreaLocation_lineEdit->setFixedHeight(heigth_Change(0.06));
    ui->NickNamelineEdit->setFixedHeight(heigth_Change(0.06));
    ui->Name_lineEdit->setFixedHeight(heigth_Change(0.06));

    ui->Man_radioButton->setFixedSize(heigth_Change(0.09),heigth_Change(0.05));
    ui->Woman_radioButton->setFixedSize(heigth_Change(0.09),heigth_Change(0.05));

    ui->PersonalSetting_Page->setLayout(ui->personlMessage_Verticalayout);

    ui->Man_radioButton->setChecked(true);

    personMessage_TcpSocket=new QTcpSocket(this);
    connect(personMessage_TcpSocket, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(personMessage_TcpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
}
void MySettingPage::sendMessage()
{
    if(sender()==personMessage_TcpSocket)
    {
        SendTcpMessage("24",personMessage_TcpSocket);
        SendTcpMessage(UserAccount,personMessage_TcpSocket);
        Friend myMessage(ui->Name_lineEdit->text(),"",ui->Age_lineEdit->text(),ui->NickNamelineEdit->text(),"",ui->AreaLocation_lineEdit->text(),true);
        if(ui->Man_radioButton->isChecked())
            myMessage.Sex="男";
        else
            myMessage.Sex="女";
        SendMyMessage(myMessage,personMessage_TcpSocket);
    }
}

void MySettingPage::receiveMessage()
{
    if(sender()==personMessage_TcpSocket)
    {
        QString Judge;
        receiveTcpMessage(Judge,personMessage_TcpSocket);
        if(Judge=="修改成功")
        {
            Friend myMessage(ui->Name_lineEdit->text(),"",ui->Age_lineEdit->text(),ui->NickNamelineEdit->text(),"",ui->AreaLocation_lineEdit->text(),true);
            if(ui->Man_radioButton->isChecked())
                myMessage.Sex="男";
            else
                myMessage.Sex="女";
            emit MyMessageChange(myMessage);
            QMessageBox::information(this, tr("提示"), tr("修改成功，点击返回"), QMessageBox::Yes);
            this->close();
        }
    }
}

void MySettingPage::SetPage(int pageNumber,Friend myMessage)
{
    ui->stackedWidget->setCurrentIndex(pageNumber);
    ui->Name_lineEdit->setText(myMessage.FriendCount);
    ui->Age_lineEdit->setText(myMessage.Age);
    ui->AreaLocation_lineEdit->setText(myMessage.Area);
    ui->NickNamelineEdit->setText(myMessage.NickName);
    if(myMessage.Sex=="男")
        ui->Man_radioButton->setChecked(true);
    else
        ui->Woman_radioButton->setChecked(true);
}

void MySettingPage::on_HeadSettingBt_clicked()
{
    ChooseFile *choosefile=new ChooseFile;
    connect(choosefile,SIGNAL(SendFileName(QString)),this,SLOT(SendPicture(QString)));
    choosefile->show();
}
void MySettingPage::SendPicture(QString filePath)
{
    qDebug()<<"启动阿桑大事大撒大事";
    QThread *thread=new QThread;                //申请新线程
    PictureThread *picturehread=new PictureThread(14,PicTureType::HeadPicture,filePath,UserAccount);     //创建线程对象
    qDebug()<<thread<<picturehread;
    picturehread->moveToThread(thread);             //把对象移到线程
    thread->start();

    emit SetHeadPicture(filePath);                   //设置头像

    QFile::remove("HeadPicture/"+UserAccount+".jpg");
    QFile::copy(filePath,"HeadPicture/"+UserAccount+".jpg");
}

MySettingPage::~MySettingPage()
{
    delete ui;
}

void MySettingPage::on_CloseBt_clicked()
{
    this->close();
}

void MySettingPage::on_MyMesaageBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MySettingPage::closeEvent(QCloseEvent *e)
{
    if(ui->stackedWidget->currentIndex()==2)
    {
        e->ignore();
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MySettingPage::on_ChangePersonMessageBt_clicked()
{
    bool is_judgeAge;
    ui->Age_lineEdit->text().toInt(&is_judgeAge);
    if(!is_judgeAge)
        QMessageBox::warning(this, tr("警告"), tr("您填写的年龄信息有误"), QMessageBox::Yes);
    else
        newConnect(personMessage_TcpSocket);
}
