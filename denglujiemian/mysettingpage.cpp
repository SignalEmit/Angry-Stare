#include "mysettingpage.h"
#include "ui_mysettingpage.h"
#include<QFileDialog>
extern QString UserAccount;
MySettingPage::MySettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySettingPage)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);                            //主界面自适应
    ui->page_1->setLayout(ui->verticalLayout_2);                    //界面一自适应
    ui->verticalLayout_2->setContentsMargins(0,0,0,heigth_Change(0.70));
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
