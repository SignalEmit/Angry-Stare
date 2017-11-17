#ifndef ORDERFORGOODS_H
#define ORDERFORGOODS_H

#include <QWidget>
#include<tcpclass.h>
namespace Ui {
class OrderForGoods;
}

class OrderForGoods : public QWidget,public TcpClass
{
    Q_OBJECT

public:
    explicit OrderForGoods(QWidget *parent = 0);
    ~OrderForGoods();

private:
    Ui::OrderForGoods *ui;
private slots:
    void ShowThisPage(int);
};

#endif // ORDERFORGOODS_H
