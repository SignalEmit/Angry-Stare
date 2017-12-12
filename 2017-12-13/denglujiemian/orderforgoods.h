#ifndef ORDERFORGOODS_H
#define ORDERFORGOODS_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include<QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpacerItem>
#include <QTextEdit>
#include<QList>
#include<QTcpSocket>
#include"tcpclass.h"
#include<receivetcpmessage.h>
namespace Ui {
class OrderForGoods;
}

class DeleteLayout
{
public:
    QGridLayout *GoodsMessage_Gridlayout;
    QLabel *PayMessage;
    QTextEdit *GoodsMessage;
    QLabel *GoodsPicture;
    QHBoxLayout *horizontalLayout;
    QLabel *ShopIcon;
    QLabel *ShopName;
    QSpacerItem *horizontalSpacer;
    QLabel *GoodsDelete;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *BuyAgain;
    QFrame *line_2;
    QFrame *line;
    QFrame *line_3;
    QWidget *baseWidget;
    DeleteLayout(QGridLayout *GoodsMessage_Gridlayout_,QLabel *PayMessage_,QTextEdit *GoodsMessage_,QLabel *GoodsPicture_,QHBoxLayout *horizontalLayout_,QLabel *ShopIcon_,QLabel *ShopName_,QSpacerItem *horizontalSpacer_,
       QLabel *GoodsDelete_,QHBoxLayout *horizontalLayout_2_,QSpacerItem *horizontalSpacer_3_,QLabel *BuyAgain_,QFrame *line_2_,
       QFrame *line_,QFrame *line_3_,QWidget *baseWidget_)
    {
        GoodsMessage_Gridlayout=GoodsMessage_Gridlayout_;   PayMessage=PayMessage_;
        GoodsMessage=GoodsMessage_; GoodsPicture=GoodsPicture_;
        horizontalLayout=horizontalLayout_; ShopIcon=ShopIcon_;
        ShopName=ShopName_; horizontalSpacer=horizontalSpacer_;
        GoodsDelete=GoodsDelete_;   horizontalLayout_2=horizontalLayout_2_;
        horizontalSpacer_3=horizontalSpacer_3_; BuyAgain=BuyAgain_;
        line_2=line_2_; line=line_;
        line_3=line_3_; baseWidget=baseWidget_;
    }
};

class OrderForGoods : public QWidget,public TcpClass
{
    Q_OBJECT

public:
    explicit OrderForGoods(QWidget *parent = 0);
    ~OrderForGoods();
private:
    int x_mousePress;                   //按下的x
    int y_mousePress;                   //按下的y
    int x_mouseRelease;                 //释放的x
    int y_mouseRelease;                 //释放的y
private:
    ReceiveOrderTcpMessage *ReceiveOrderMessage;
    bool is_FirstShow=true;                                     //是不是第一次显示
    Ui::OrderForGoods *ui;
    QTcpSocket *tcpSocket_buyGoodsMessage;                      //接收购买的商品的信息
    QVBoxLayout *baseLayout;                    //底层排布
    QWidget *base;
    QList<DeleteLayout> theListOfDeleteLayout;                              //删除并查找排布器
    QList<PayMessage> PayMessageList;                                       //支付信息List
private:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    void showEvent(QShowEvent *e);
    QWidget* SetGoods(QString,QString,QString,QString,QString);                            //设置商品信息
private slots:
    void ShowWholeGoods(QList<PayMessage> _list);
    void SetGoodsPicture(QString);
    void ShowThisPage(int);
    void sendMessage();
    void receiveMessage();
    void on_Obligation_clicked();
};

#endif // ORDERFORGOODS_H
