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
    this->setLayout(ui->verticalLayout);
    this->move((width_Change(1)-this->width())/2,(heigth_Change(1)-this->height())/2);

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
    else if(dialogloginJudge=="您已被强制下线")
        ui->LogInlabel->setText("您已被强制下线");
}
void DialogLogIn::closeEvent(QCloseEvent *e)
{
    if(dialogloginJudge=="widgetpage")
        e->accept();
    else if(dialogloginJudge=="ChatPage")
        e->accept();
    else if(dialogloginJudge=="您的账号以异地登录")
        e->ignore();
    else if(dialogloginJudge=="您已被强制下线")
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
    else if(dialogloginJudge=="您已被强制下线")
        exit(0);
    else
        this->close();
}

