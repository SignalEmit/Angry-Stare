#include "widget.h"
#include "ui_widget.h"
#include<QDesktopWidget>
#include<QDebug>
QString UserAccount;                //使用者账号
QString dialogloginJudge;           //提示界面判断
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ShowComboBox=false;
    FirstShowAutoLogin=true;
    ui->Account_lineedit->lineEdit()->setPlaceholderText("请输入账号");
    ui->remmberPassword->setChecked(false);
    ui->autoLogin->setChecked(false);
    /**************界面一装配***********/
    ui->page->setLayout(ui->verticalLayout_2);
    ui->verticalLayout->addWidget(ui->stackedWidget);
    this->setLayout(ui->verticalLayout);

    ui->photo->setFixedSize(heigth_Change(0.15), heigth_Change(0.15));

    /**************界面一微调***********/
    ui->verticalLayout_3->setContentsMargins(width_Change(0.1),0,width_Change(0.1),0);
    ui->horizontalLayout->setContentsMargins(width_Change(0.265),0,width_Change(0.185),0);
    ui->verticalLayout_4->setContentsMargins(width_Change(0.7),0,0,0);
    ui->verticalLayout_2->setContentsMargins(0,heigth_Change(0.3),0,heigth_Change(0.3));

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
   // ui->Check_RegisterLogInBt->setFixedWidth(width_Change(0.1));
  //  ui->SendAuthCodeBt__RegisterLogIn->setFixedWidth(width_Change(0.1));

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

    ui->Account_lineedit->setStyleSheet(tr("QComboBox::drop-down{border-radius:5px;border-radius:5px;border:2px solid #DCE4EC;width:20px;image:url(:/ICON/picture/down_arrow.ico);width:%0}"
                                           "QComboBox{border-style:none;padding:6px;border-radius:5px;border:2px solid #DCE4EC;}"
                                           "QComboBox:focus{border:2px solid #1ABC9C;}"
                                            ).arg(QString::number(ui->Account_lineedit->height())));

     /*********************************数据库*************************/
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");

    QFile file("MyDateBase.db");
    if(file.exists())
    {

        db.setDatabaseName("MyDateBase.db");
    }
    else
    {
        db.setDatabaseName("MyDateBase.db");
        db.open();
        QSqlQuery BuildNewTable;
        BuildNewTable.exec("CREATE TABLE [LoginUserTable]("
                           "[UserAccount] VARCHAR(50) NOT NULL,"
                           "[Password] VARCHAR(30), "
                           "[is_AutomaticLogon] BOOL(2),"
                           "[is_Login] BOOL(2),"
                           "UNIQUE([UserAccount]));");
        BuildNewTable.exec("CREATE TABLE [SearchHistory]("
                           "[History] VARCHAR(50),"
                           "UNIQUE([History]));");

       db.close();
    }

}

void Widget::showEvent(QShowEvent *e)
{
    QString FirstShowAccount;
    bool is_FirstShowAccountLogin;
    db.open();
    QSqlQuery addItemInComboBox;
    addItemInComboBox.exec(tr("select * from LoginUserTable"));
    while(addItemInComboBox.next())
    {
        ui->Account_lineedit->addItem(addItemInComboBox.value(0).toString());
        qDebug()<<addItemInComboBox.value(0).toString();

        if(addItemInComboBox.value(3).toBool())
        {
            ui->remmberPassword->setChecked(true);
            ui->autoLogin->setChecked(addItemInComboBox.value(2).toBool());
            ui->Password_lineedit->setText(addItemInComboBox.value(1).toString());
            FirstShowAccount=addItemInComboBox.value(0).toString();
            is_FirstShowAccountLogin=addItemInComboBox.value(2).toBool();
        }

    }
    db.close();
    ShowComboBox=true;
    ui->Account_lineedit->lineEdit()->setText(FirstShowAccount);
    if(ui->Account_lineedit->lineEdit()->text()!="")
     {   if(is_FirstShowAccountLogin)
            newConnect(tcpSocket_Login);                //发送自动登录套接字
         else
            FirstShowAutoLogin=false;
    }
    else
        FirstShowAutoLogin=false;
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
    QMessageBox::warning(this, tr("警告"), tr("请检查您的网络"), QMessageBox::Yes);
    FirstShowAutoLogin=false;
}
void Widget::sendMessage()
{
    if(sender()==tcpSocket_Login)
    {
        SendTcpMessage("0",tcpSocket_Login);           //发送登录
        SendTcpMessage(ui->Account_lineedit->currentText(),tcpSocket_Login);
        SendTcpMessage(ui->Password_lineedit->text(),tcpSocket_Login);
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
        receiveTcpMessage(Judge,tcpSocket_Login);
        tcpSocket_Login->disconnectFromHost();
        if(Judge=="密码账号正确")
        {

            qDebug()<<"asd";
            UserAccount=ui->Account_lineedit->currentText();           //给纪录的账号赋值账号赋

            QString is_AutoLogin=QString::number(ui->autoLogin->isChecked());
            if(ui->remmberPassword->isChecked())
            {
                db.open();
                QSqlQuery UpdataDatabase;
                UpdataDatabase.exec("update LoginUserTable set is_Login='false'");      //先将登录赋值false

                if(!UpdataDatabase.exec(tr("insert into LoginUserTable(UserAccount,Password,is_AutomaticLogon,is_Login) values('%0','%1',%2,'true')").arg(ui->Account_lineedit->currentText(),ui->Password_lineedit->text(),is_AutoLogin)))
                    UpdataDatabase.exec(tr("update LoginUserTable set Password='%0',is_AutomaticLogon=%1,is_Login='true' where UserAccount='%3'").arg(ui->Password_lineedit->text(),is_AutoLogin,ui->Account_lineedit->currentText()));
                db.close();
            }

            if(FirstShowAutoLogin)
            {
                //当做第一次登录的自动登录时
                dialogloginJudge="爱啥啥";             //为了和那些页面区别还有关闭的时候方便调用ignore（）
                DialogLogIn TimeOutPage;
                connect(&TimeOutPage,SIGNAL(ShowInterFacePage()),&newwidget,SLOT(show()));
                TimeOutPage.exec();

                FirstShowAutoLogin=false;
            }
            else
                newwidget.show();
        }
        else
            LogInDialog("账号密码不正确请重新输入");
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
    delete ui;
    //delete tcpSocket_Login;
    //delete tcpSocket_CheckAccountRegisterLogIn;
    //delete tcpSocket_RegisterLogIn;
    //delete tcpSocket_ForgetPassword;
}
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/
/*****************************************网络***************************/

void Widget::on_Login_clicked()
{
    if(ui->Account_lineedit->currentText().isEmpty()||ui->Password_lineedit->text().isEmpty())
        LogInDialog("账号或密码为空");
    else
        newConnect(tcpSocket_Login);
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


void Widget::on_Account_lineedit_currentTextChanged(const QString &arg1)
{
    QFile headfile(tr("HeadPicture/%0.jpg").arg(arg1));
    if(headfile.exists())
        ui->photo->setStyleSheet(tr("border-image: url(./HeadPicture/%0)").arg(arg1));
    else
        ui->photo->setStyleSheet("border-image: url(:/sucai/HeadPicture.ico)");


    if(ShowComboBox)                    //如果这个出现过了
    {
        db.open();
        QSqlQuery addItemInComboBox;
        addItemInComboBox.exec(tr("select * from LoginUserTable where UserAccount='%0'").arg(arg1));
        if(addItemInComboBox.next())
        {
            ui->Account_lineedit->lineEdit()->setText(addItemInComboBox.value(0).toString());
            ui->remmberPassword->setChecked(true);
            ui->autoLogin->setChecked(addItemInComboBox.value(2).toBool());
            ui->Password_lineedit->setText(addItemInComboBox.value(1).toString());
        }
        else
        {
            ui->remmberPassword->setChecked(false);
            ui->autoLogin->setChecked(false);
        }
        db.close();
    }
}


