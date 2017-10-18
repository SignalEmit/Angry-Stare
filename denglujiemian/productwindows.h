#ifndef PRODUCTWINDOWS_H
#define PRODUCTWINDOWS_H

#include <QWidget>
#include<QTcpSocket>
#include <QtNetwork>
#include"chat.h"
#include"tcpclass.h"

namespace Ui {
class ProductWindows;
}

class ProductWindows : public QWidget,public TcpClass
{
    Q_OBJECT

public:
    explicit ProductWindows(QWidget *parent = 0);
    ~ProductWindows();

private slots:

    void on_FarmBt_clicked();
    void on_PartTimeJob_clicked();
    void on_SendPicture_clicked();

    void on_FarmProductOkBt_clicked();

    void on_Yes_CostSender_radioBt_clicked();

    void on_No_CostSender_radioBt_clicked();

private:
    Chat FriendChatPage;
    Ui::ProductWindows *ui;
private:

    void showEvent(QShowEvent *e);
    void closeEvent(QCloseEvent *e);



private:
    QTcpSocket *SendProductForDataBase;             //发送商品信息进入数据库
    QTcpSocket *UpdataFarmProduct;                 //向农业数据库内添加新条目
    QTcpSocket *ReceiveSeller;                      //接收联系人信息
private slots:
    void sendMessage();
    void receiveMessage();
    void on_BackBt_clicked();
    void on_minusBt_clicked();
    void on_addBt_clicked();
    void on_ContactTheSellerBt_clicked();
    void on_BuyNowBt_clicked();

    void on_PlaceOrderBt_clicked();
};

#endif // PRODUCTWINDOWS_H
