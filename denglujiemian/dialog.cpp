#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //ui->label_2->setFixedSize(200,200);
    this->setLayout(ui->verticalLayout);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Btclick()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Btclick()));
}

void Dialog::Btclick()
{
    if(sender()==ui->pushButton_2)
    {
        emit ExitProduct();
    }
    else if(sender()==ui->pushButton)
     {

        this->close();
    }
}

Dialog::~Dialog()
{
    delete ui;
}
