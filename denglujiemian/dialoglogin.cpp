#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include"widget.h"
#include<QDesktopWidget>
extern QString dialogloginJudge;           //提示界面判断
DialogLogIn::DialogLogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogIn)
{
    ui->setupUi(this);
    TimeCount=0;
    this->setLayout(ui->verticalLayout);
    this->move(width_Change(0.28),heigth_Change(0.4));
    connect(&CountTime,SIGNAL(timeout()),this,SLOT(TimeEnd()));
}
void DialogLogIn::showEvent(QShowEvent *e)
{
    if(dialogloginJudge=="widgetpage")
    {
        Widget *LogInPtr=(Widget*)parentWidget();
        ui->LogInlabel->setText(LogInPtr->LogInLabel);
    }
    else if(dialogloginJudge=="ChatPage")
    {
        Chat *LogInPtr=(Chat*)parentWidget();
        ui->LogInlabel->setText(LogInPtr->LogInLabel);
    }
    else if(dialogloginJudge=="您的账号以异地登录")
        ui->LogInlabel->setText("您的账号异地登录");
    else
    {
        CountTime.start(1000);
        ui->LogInlabel->setText(QString::number(5)+"秒后进入系统\n您可以点击下方取消按钮取消");
        ui->pushButton->setText("取消");
    }
}
void DialogLogIn::closeEvent(QCloseEvent *e)
{
    if(dialogloginJudge=="widgetpage")
        e->accept();
    else if(dialogloginJudge=="ChatPage")
        e->accept();
    else if(dialogloginJudge=="您的账号以异地登录")
        e->ignore();
    else
        e->accept();
}

DialogLogIn::~DialogLogIn()
{
    delete ui;
}

void DialogLogIn::on_pushButton_clicked()
{
    if(dialogloginJudge=="widgetpage")
        this->close();
    else if(dialogloginJudge=="ChatPage")
        this->close();
    else if(dialogloginJudge=="您的账号以异地登录")
        exit(0);
    else
        this->close();
}

void DialogLogIn::TimeEnd()
{
    TimeCount++;
    ui->LogInlabel->setText(QString::number(5-TimeCount)+"秒后进入系统\n您可以点击下方取消按钮取消");
    if(5-TimeCount==0)
    {
        TimeCount=0;
        CountTime.stop();
        emit ShowInterFacePage();
        this->close();
    }
}
