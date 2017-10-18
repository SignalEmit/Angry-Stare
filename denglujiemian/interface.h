#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include<QScrollArea>
#include<QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include<QGridLayout>
#include<QLabel>
#include<QMouseEvent>
#include<QPushButton>
#include<QCloseEvent>
#include<QMessageBox>

#include <QtNetwork>
#include<QTcpSocket>
#include<QFile>
#include<QByteArray>
#include <QDataStream>
#include<QDir>
#include<QFile>
#include<QThread>
#include<QTimer>
#include"picturethread.h"
#include"pageme.h"
#include"dialog.h"
#include"productwindows.h"
#include"tcpclass.h"
#include"search.h"
#include"dialoglogin.h"
#include"mysettingpage.h"
namespace Ui {
class Interface;
}

class Interface : public QWidget,public TcpClass
{
    Q_OBJECT
signals:
    void RecommendPictrueReceiveFinsh();         //图片接收完成信号
    void ShowPicture();
    void PageMePageShowChatMessage(QString,QString,bool);                    //好友列表及消息显示界面显示消息

private:
    ProductWindows FarmProductPage;
private:
    QTimer ChangePicture;                   //定时器
    int PictureChangeCount;                         //图片改变计数器
    QLabel *ChangePageLabel[7];                   //图片改变的按钮
public:
    explicit Interface(QWidget *parent = 0);
    ~Interface();

private slots:
    void PictureChangeSlot();               //图片自动改变的事件
    void SetRecommendPicture(QString);
    void SetUserHeadPicture(QString);
private:
    DialogLogIn dialogloginpage;            //异地登录提示界面
    QScrollArea *ScrollAreaPage1;                       //滑块区
    QScrollArea *ScrollAreaPage2;                       //滑块区
    QWidget *page1;                                     //
private:
    int newsreel_Button;

    Ui::Interface *ui;
    QScrollBar *m_scrollBarV[2];                           //绑定滑动事件
    /************************Text专用***************************/

    bool is_FristShowTheCount;                  //是否第一次显示这些数组
    QLabel **Label1_Text;
    QLabel **Label2_Text;
    QLabel **Label3_Text;
    QLabel **Label4_Text;
    QWidget **Widget_Text;
    QGridLayout **GridLayout_Text;



    //Search search;
    PageMe pageme;
    Dialog dialog_close;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    int press_y[2];
    int move_y[2];
    int release_y[2];
    QDateTime pressDateTime[2];
    QPropertyAnimation *animation[2];


    void closeEvent(QCloseEvent *e);
    int x_mousePress;                   //按下的x
    int y_mousePress;                   //按下的y
    int x_mouseRelease;                 //释放的x
    int y_mouseRelease;                 //释放的y
    bool Judge=true;                         //限制进入循环状态防之多点

private slots:
    void ChangePage(int pageNumber);
    void on_Other_clicked();
    void ReceivePictureAndShow();           //显示后再接收图片
    void SetHeadPictureIcon(QString);       //设置头像

    /*****************************网络*****************************/
    /*****************************网络*****************************/
    /*****************************网络*****************************/
private:


    QList<Procuct> RecommendProcuctList;
    QTcpSocket *Offline;             //离线
    QTcpSocket *tcpSocket_Recommend;
    QTcpSocket *Online;      //发送上线的请求

    int SendRecommendPictureCount;          //发送推荐列表图片的计数器

private:
    void showEvent(QShowEvent *event);
private slots:
    void offLine();                //发送下线通知服务器
    void displayError(QAbstractSocket::SocketError);
    void receiveMessage();
    void sendMessage();



    void on_SearchBt_clicked();
    void on_MeBt_clicked();
    void on_homePageBt_clicked();
    void on_recommendBt_clicked();
    void on_Logo_clicked();
    void on_PostYourWant_clicked();

    void on_SetUserAccount_clicked();
    void on_stackedWidget_3_currentChanged(int arg1);
};

#endif // INTERFACE_H
