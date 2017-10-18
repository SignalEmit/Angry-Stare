#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include<QListWidgetItem>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include<QTcpSocket>
#include"tcpclass.h"
#include<QLabel>
#include<QGridLayout>

#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>


#include<productwindows.h>
#include"picturethread.h"
namespace Ui {
class Search;
}

class Search : public QWidget,public TcpClass
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = 0);
    ~Search();
private slots:
    void on_SearchLineEdit_textChanged(const QString &arg1);

    void on_listWidget_itemClicked(QListWidgetItem *item);



private:
    Ui::Search *ui;

private:
    QLabel *SearchEmpty;                //查询为空字符串
    /*********************************数据库*************************/
    QSqlDatabase db;                //数据库


    int press_y[3];
    int move_y[3];
    int release_y[3];
    QScrollBar *m_scrollBarV[3];
    QDateTime pressDateTime[3];
    QPropertyAnimation *animation[3];
private:
    ProductWindows FarmProductPage;                     //商品详情界面
    QTcpSocket *SearchMessage;                              //关键字查询商品
    QTcpSocket *ReceiveProductMessage;                  //接收简单的商品信息
private:

    int x_mousePress;                   //按下的x
    int y_mousePress;                   //按下的y
    int x_mouseRelease;                 //释放的x
    int y_mouseRelease;                 //释放的y


    QString ReceiveNeedShowProductMessage;                  //接收需要显示的商品信息
    QList<Procuct> ProductList;                             //商品链表

    /************************Text专用***************************/
    bool is_FristShowTheCount;                      //是否第一次显示初始化数组
    QWidget *widget;                    //放控件的地方

    QLabel **Label1_Text;
    QLabel **Label2_Text;
    QLabel **Label3_Text;
    QLabel **Label4_Text;
    QWidget **Widget_Text;
    QGridLayout **GridLayout_Text;

    QGridLayout *gridLayout;                        //排布器

    int UpdataNumber;                               //注册到第几个控件更新到

private:
    void OperatDataBaseHistory(QString str);                           //删除数据库中的历史记录
    void UpdataSearchProductView();                         //产品界面的更新操作
    void ReceiveProductPicture();                           //接收图片
    void showEvent(QShowEvent *e);
    void closeEvent(QCloseEvent *e);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void sendMessage();
    void receiveMessage();
    void SetRecommendPicture(QString);                      //设置图片
    void on_SearchListWidget_itemClicked(QListWidgetItem *item);
    void on_ClearListWidget_clicked();
    void on_SearchBt_clicked();
    void on_BackBt_clicked();
};

#endif // SEARCH_H
