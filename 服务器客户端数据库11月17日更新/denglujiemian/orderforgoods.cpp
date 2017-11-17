#include "orderforgoods.h"
#include "ui_orderforgoods.h"

OrderForGoods::OrderForGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderForGoods)
{
    ui->setupUi(this);
    ui->Bt_widget->setLayout(ui->horizontalLayout_Bts);                     //将按钮布局绑定到按钮页面上
    ui->GoBackBt->setFixedSize(width_Change(0.1),width_Change(0.1));
}
void OrderForGoods::ShowThisPage(int PageNumber)
{
    ui->stackedWidget->setCurrentIndex(PageNumber);
}

OrderForGoods::~OrderForGoods()
{
    delete ui;
}
