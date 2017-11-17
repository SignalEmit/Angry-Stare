#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"mythread.h"
#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QtNetWork>
#include<QTcpServer>
#include <QStandardItemModel>
#include<QList>
#include<QSqlQueryModel>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void monitor();
    void on_OkBt_clicked();
    void on_DeleteBt_clicked();
    void on_pushButton_clicked();
    void on_RecommendPage_clicked();
    void on_OnlinePeople_clicked();
    void addOnlineWidgetList(OnlinePeople);      //在在线列表中添加
    void offPeople(QString);                        //下线用户
    void on_dateLookUp_clicked();

    void on_UpdateDateBase_clicked();

    void on_DatabaseTableView_doubleClicked(const QModelIndex &index);

    void on_CompulsoryOffline_clicked();

    void on_OnlinePeoplelistWidget_clicked(const QModelIndex &index);

private:
    void adGoodsItem(const QString &data);
    void SendTcpMessage(QString message,QTcpSocket *tcpSocket_1);
private:
    QList<QString> NeedOffLine;
    QList<Procuct> ProcuctList;                     //商品list
    QStandardItemModel *RecommendListModel;
    QTcpServer *tcpServer;
    QSqlDatabase db;
    MyThread *mythread[10000];
    int ThreadCount;
private:
    Ui::Widget *ui;
    //发送链表
};

#endif // WIDGET_H
