#include "widget.h"
#include "ui_widget.h"
#include<QDesktopWidget>
#include<QDebug>
extern QString UserAccount;                //使用者账号
QString dialogloginJudge;           //提示界面判断
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    is_click_loginbt=false;                 //未点击登录按键
    ui->Account_lineedit->setStyleSheet(tr("border:none;background-color: rgba(0, 0, 0, 0);margin-left:%0px").arg(heigth_Change(0.02)));
    ui->Password_lineedit->setStyleSheet(tr("border:none;background-color: rgba(0, 0, 0, 0);margin-left:%0px").arg(heigth_Change(0.02)));
    ui->account_widget->setFixedHeight(heigth_Change(0.08));
    ui->passward_widget->setFixedHeight(heigth_Change(0.08));
    ui->account_widget->setLayout(ui->account_horizontalLayout);
    ui->passward_widget->setLayout(ui->passward_horizontalLayout);
    ui->account_phone->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->passward_phone->setFixedSize(heigth_Change(0.06),heigth_Change(0.06));
    ui->Account_lineedit->setFixedHeight(heigth_Change(0.06));
    ui->Password_lineedit->setFixedHeight(heigth_Change(0.06));
    ui->Account_lineedit->installEventFilter(this);
    ui->Password_lineedit->installEventFilter(this);
    ui->label_18->setFixedHeight(heigth_Change(0.01));
    ui->page->setLayout(ui->verticalLayout_2);
    ui->Login->setFixedHeight(heigth_Change(0.06));
    /**************界面一装配***********/
    ui->verticalLayout->addWidget(ui->stackedWidget);
    this->setLayout(ui->verticalLayout);


    /**************界面一微调***********/
    ui->horizontalLayout->setContentsMargins(width_Change(0.265),0,width_Change(0.185),0);

    /**************界面三装配***********/
    ui->page_2->setLayout(ui->gridLayout_2);
    /**************界面三微调***********/
   // ui->lineEdit_11->hide();
    ui->gridLayout_2->setContentsMargins(width_Change(0.05),heigth_Change(0.1),width_Change(0.05),heigth_Change(0.5));;

    /**************界面二装配***********/
    ui->page_4->setLayout(ui->gridLayout_3);
    ui->gridLayout_3->setContentsMargins(width_Change(0.05),heigth_Change(0.1),width_Change(0.05),heigth_Change(0.5));
    connect(ui->register1,SIGNAL(clicked(bool)),this,SLOT(pushbutton_clicked()));
    connect(ui->forgetPWBt,SIGNAL(clicked(bool)),this,SLOT(pushbutton_clicked()));
    connect(ui->backBtPage4,SIGNAL(clicked(bool)),this,SLOT(pushbutton_clicked()));
    connect(ui->backBtPage2,SIGNAL(clicked(bool)),this,SLOT(pushbutton_clicked()));
    connect(ui->Login,SIGNAL(clicked(bool)),this,SLOT(pushbutton_clicked()));


    ui->pushButton->setFixedWidth(width_Change(0.1));               //注册按钮大小


    /****************************************************/
    /****************************************************/
    /**********************网络**************************/
    /****************************************************/
    /****************************************************/

    //登录套接字
    tcpSocket_Login = new QTcpSocket(this);             //及其用法
    connect(tcpSocket_Login, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_Login, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_Login, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    //注册套接字及其用法
    tcpSocket_RegisterLogIn = new QTcpSocket(this);
    connect(tcpSocket_RegisterLogIn, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_RegisterLogIn, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_RegisterLogIn, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    //检查账号接字及其用法
    tcpSocket_CheckAccountRegisterLogIn= new QTcpSocket(this);
    connect(tcpSocket_CheckAccountRegisterLogIn, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_CheckAccountRegisterLogIn, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_CheckAccountRegisterLogIn, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    //忘记密码套接字及其用法
    tcpSocket_ForgetPassword= new QTcpSocket(this);
    connect(tcpSocket_ForgetPassword, SIGNAL(connected()), this, SLOT(sendMessage()));
    connect(tcpSocket_ForgetPassword, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(tcpSocket_ForgetPassword, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));



     /*********************************数据库*************************/
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");

    db.open();
    QFile file("MyDateBase.db");
    if(file.exists())
    {

        db.setDatabaseName("MyDateBase.db");
    }
    else
    {
        db.setDatabaseName("MyDateBase.db");
        QSqlQuery BuildNewTable;
        BuildNewTable.exec("CREATE TABLE [LoginUserTable]("
                           "[UserAccount] VARCHAR(50) NOT NULL,"
                           "[Password] VARCHAR(30), "
                           "[is_Login] BOOL(2),"
                           "UNIQUE([UserAccount]));");
        BuildNewTable.exec("CREATE TABLE [SearchHistory]("
                           "[History] VARCHAR(50),"
                           "UNIQUE([History]));");

    }
    db.open();

    /*********************************数据库*************************/
    db.open();
    QSqlQuery check("select UserAccount from LoginUserTable where is_Login='true'");
    check.exec();
    if(check.next())
    {
        newConnect(tcpSocket_Login);
    }
    db.close();


}

void Widget::showEvent(QShowEvent *e)
{
    ui->Login->setEnabled(true);
}

void Widget::pushbutton_clicked()
{
    if(sender()==ui->register1)
        ui->stackedWidget->setCurrentIndex(1);
    else if(sender()==ui->forgetPWBt)
        ui->stackedWidget->setCurrentIndex(2);
    else if(sender()==ui->backBtPage4)
        ui->stackedWidget->setCurrentIndex(0);
    else if(sender()==ui->backBtPage2)
        ui->stackedWidget->setCurrentIndex(0);
}


/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
void Widget::displayError(QAbstractSocket::SocketError)
{
    if(sender()==tcpSocket_Login)
    {
        this->show();
        ui->Login->setEnabled(true);
        QMessageBox::warning(this, tr("警告"), tr("请检查您的网络"), QMessageBox::Yes);
    }
}
void Widget::sendMessage()
{
    if(sender()==tcpSocket_Login)
    {
        if(is_click_loginbt)
        {
            SendTcpMessage("0",tcpSocket_Login);           //发送登录
            SendTcpMessage(ui->Account_lineedit->text(),tcpSocket_Login);
            SendTcpMessage(ui->Password_lineedit->text(),tcpSocket_Login);
        }
        else                //未点击登录按键
        {
            db.open();
            QSqlQuery check("select UserAccount,Password from LoginUserTable where is_Login='true'");
            check.exec();
            if(check.next())
            {
                ui->Account_lineedit->setText(check.value("UserAccount").toString());
                ui->Password_lineedit->setText(check.value("Password").toString());
                SendTcpMessage("0",tcpSocket_Login);           //发送登录
                SendTcpMessage(check.value("UserAccount").toString(),tcpSocket_Login);
                SendTcpMessage(check.value("Password").toString(),tcpSocket_Login);
            }
            db.close();
        }
    }
    else if(sender()==tcpSocket_CheckAccountRegisterLogIn)
    {
        SendTcpMessage("1",tcpSocket_CheckAccountRegisterLogIn);//发送检测账号
        SendTcpMessage(ui->Account_RegisterLogIn->text(),tcpSocket_CheckAccountRegisterLogIn);
    }
    else if(sender()==tcpSocket_RegisterLogIn)
    {
        SendTcpMessage("2",tcpSocket_RegisterLogIn);//发送注册账号
        SendTcpMessage(ui->Account_RegisterLogIn->text(),tcpSocket_RegisterLogIn);
        SendTcpMessage(ui->Passward_RegisterLogIn->text(),tcpSocket_RegisterLogIn);
        SendTcpMessage(ui->PhoneNumber_RegisterLogIn->text(),tcpSocket_RegisterLogIn);
    }
    else if(sender()==tcpSocket_ForgetPassword)
    {
        SendTcpMessage("3",tcpSocket_ForgetPassword);//发送忘记密码
        SendTcpMessage(ui->Account_ForgetPassword->text(),tcpSocket_ForgetPassword);
        SendTcpMessage(ui->Password_ForgetPassword->text(),tcpSocket_ForgetPassword);
        SendTcpMessage(ui->PhoneNumber_ForgetPassword->text(),tcpSocket_ForgetPassword);
    }
}

void Widget::receiveMessage()
{
    QString Judge;              //判断
    if(sender()==tcpSocket_Login)                               //检测登录
    {
        tcpSocket_Login->disconnectFromHost();
        receiveTcpMessage(Judge,tcpSocket_Login);
        if(Judge=="密码账号正确")
        {

            qDebug()<<"asd";
            UserAccount=ui->Account_lineedit->text();           //给纪录的账号赋值账号赋

            db.open();
            QSqlQuery UpdataDatabase;
            UpdataDatabase.exec("update LoginUserTable set is_Login='false' where is_Login='true'");      //先将登录赋值false

            if(!UpdataDatabase.exec(tr("insert into LoginUserTable(UserAccount,Password,is_Login) values('%0','%1','true')").arg(ui->Account_lineedit->text(),ui->Password_lineedit->text())))
                UpdataDatabase.exec(tr("update LoginUserTable set Password='%0',is_Login='true' where UserAccount='%1'").arg(ui->Password_lineedit->text(),ui->Account_lineedit->text()));
            db.close();

            Interface *newwidget=new Interface;
            connect(newwidget,SIGNAL(loginWidgetShow()),this,SLOT(show()));
            this->close();
            newwidget->show();
            theInterFaceList.push_back(newwidget);

        }
        else
        {
            this->show();
            ui->Login->setEnabled(true);
            LogInDialog("账号密码不正确请重新输入");
        }
    }
    else if(sender()==tcpSocket_CheckAccountRegisterLogIn)      //检测账号是否存在
    {
        receiveTcpMessage(Judge,tcpSocket_CheckAccountRegisterLogIn);
        if(Judge=="账号存在")
            ui->Check_RegisterLogInBt->setText("账号存在");
        else if(Judge=="账号不存在")
            ui->Check_RegisterLogInBt->setText("账号不存在");
    }
    else if(sender()==tcpSocket_RegisterLogIn)      //检测账号是否存在
    {
        receiveTcpMessage(Judge,tcpSocket_RegisterLogIn);
        if(Judge=="账号存在")
            LogInDialog("账号存在,难受");
        else if(Judge=="注册成功")
        {
            LogInDialog("消息提示注册成功");
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else if(sender()==tcpSocket_ForgetPassword)//检测忘记密码
    {
        receiveTcpMessage(Judge,tcpSocket_ForgetPassword);
        if(Judge=="账号不存在")
            LogInDialog("账号不存在");
        else if(Judge=="手机号错误")
            LogInDialog("手机号错误");
        else if(Judge=="修改成功")
        {
            LogInDialog("修改成功");
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}
Widget::~Widget()
{
    for(int i=0;i<theInterFaceList.size();i++)
        delete theInterFaceList[i];
    theInterFaceList.clear();
    delete ui;
}
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/

void Widget::on_Login_clicked()
{
    if(ui->Account_lineedit->text().isEmpty()||ui->Password_lineedit->text().isEmpty())
        LogInDialog("账号或密码为空");
    else
    {
        ui->Login->setEnabled(false);
        is_click_loginbt=true;
        newConnect(tcpSocket_Login);
    }
}

void Widget::on_Check_RegisterLogInBt_clicked()
{
    if(ui->Account_RegisterLogIn->text().isEmpty())
        LogInDialog("账号为空");
    else
        newConnect(tcpSocket_CheckAccountRegisterLogIn);
}

void Widget::on_Ok_RegisterLogInBt_clicked()
{
    if(ui->Account_RegisterLogIn->text().isEmpty()||ui->Passward_RegisterLogIn->text().isEmpty()||ui->PasswordAgain_RegisterLogIn->text().isEmpty()||ui->PhoneNumber_RegisterLogIn->text().isEmpty())
        LogInDialog("所有的东西都应填写");
    else if(ui->PhoneNumber_RegisterLogIn->text().length()!=11)
        LogInDialog("请输入正确的手机号码");
    else if(ui->Passward_RegisterLogIn->text()!=ui->PasswordAgain_RegisterLogIn->text())
        LogInDialog("两次输入的密码不一致");
    else
        newConnect(tcpSocket_RegisterLogIn);
}

void Widget::on_Ok_ForgetPassword_clicked()
{
    if(ui->Account_ForgetPassword->text().isEmpty()||ui->Password_ForgetPassword->text().isEmpty()||ui->PasswordAgain_ForgetPassword->text().isEmpty()||ui->PhoneNumber_ForgetPassword->text().isEmpty())
        LogInDialog("哪个项目为空");
    else if(ui->Password_ForgetPassword->text()!=ui->PasswordAgain_ForgetPassword->text())
        LogInDialog("两次输入的密码不一致");
    else
        newConnect(tcpSocket_ForgetPassword);
}
 void Widget::LogInDialog(QString Label)
 {
     dialogloginJudge="widgetpage";
     LogInLabel=Label;
     DialogLogIn showDialog(this);
     showDialog.exec();
 }

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->Account_lineedit)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            ui->passward_widget->setStyleSheet("border-bottom:1px solid gray;");
            ui->account_widget->setStyleSheet("border-bottom:1px solid rgb(0, 255, 123);");
            return true;
        }
    }

    if(obj==ui->Password_lineedit)
    {
        {
            if(event->type()==QEvent::MouseButtonPress)
            {
                ui->passward_widget->setStyleSheet("border-bottom:1px solid rgb(0, 255, 123);");
                ui->account_widget->setStyleSheet("border-bottom:1px solid gray;");
                return true;
            }
        }
    }
    return false;
}
void Widget::closeEvent(QCloseEvent *event)
{

}
