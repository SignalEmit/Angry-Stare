#include "choosefile.h"
#include "ui_choosefile.h"
ChooseFile::ChooseFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseFile)
{
    ui->setupUi(this);

    /***********************************滑动区赋值*******************************/
    press_y= 0;
    move_y= -1;
    release_y = 0;
    animation = new QPropertyAnimation();

    /*********************************滑动区绑定**********************************/
    m_scrollBarV=ui->listWidget->verticalScrollBar();
    QObjectList objectList = ui->listWidget->children();
    for(int i = 0; i < objectList.count(); i++)
    {
        objectList.at(i)->setObjectName(QStringLiteral("qt_scrollarea_viewport"));
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport")
        {
            objectList.at(i)->installEventFilter(this);
        }
    }


    this->setLayout(ui->gridLayout);
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotShowDir(QListWidgetItem *)));

    ui->listWidget->setIconSize(QSize(width_Change(0.28),width_Change(0.28)));
}
bool ChooseFile::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->objectName()!="qt_scrollarea_viewport")
        return false;
    else if("qt_scrollarea_viewport" == obj->objectName())
        return SlidingRegin(event,obj,m_scrollBarV,animation,pressDateTime,press_y,move_y,release_y);
}

ChooseFile::~ChooseFile()
{
    delete ui;
}
void ChooseFile::closeEvent(QCloseEvent *e)
{
    root="";
}

void ChooseFile::slotShowDir(QListWidgetItem *item)
{
    if(item->text().right(4)!=".jpg"&&item->text().right(4)!=".png")
    {
        root += item->text()+"/";

        qDebug()<<root;
        QDir dir;
        dir.setPath(root);
        slotShow(dir);
    }
    else
    {
        root += item->text();

        qDebug()<<root;
/*
        QString endFile=root.right(4);                                      //截取后4位
        QFile a(root);                                                      //绑定文件
        QString newFilePath="HeadPicture/"+UserAccount+endFile;                    //新文件路径
        a.copy(newFilePath);                                        //新图片路径
  */
        emit SendFileName(root);
        this->close();
    }
}
/**********文件过滤*********/
void ChooseFile::slotShow(QDir dir) {

    QStringList string;
    string << "*.jpg"<<"*.png";
    QFileInfoList list = dir.entryInfoList(QDir::AllDirs| QDir::NoDotAndDotDot);
    QFileInfoList list1 = dir.entryInfoList(string);
    showFileInfoList(list+ list1);
}
/**********过滤显示*********/
void ChooseFile::showFileInfoList(QFileInfoList list) {
    ui->listWidget->clear();

    for (unsigned int i = 0; i<list.count(); i++) {
        QFileInfo temFileinfo = list.at(i);
        if (temFileinfo.isDir()) {
            QIcon icon(":/picture/Dir.ico");
            QString fileName = temFileinfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
            tmp->setSizeHint (QSize(width_Change(0.3),width_Change(0.3)));
            QFont ZiSize;
            ZiSize.setPointSize(15);

            tmp->setFont(ZiSize);
            ui->listWidget->addItem(tmp);
        }
        else {
            QString fileName = temFileinfo.fileName();

            QFile file(root+fileName);
            QIcon icon;
            if(file.size()>800*1024)
                icon.addFile(":/picture/Picture.ico");
            else
                icon.addFile(root+fileName);

            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
            tmp->setSizeHint (QSize(width_Change(0.3),width_Change(0.3)));

            QFont ZiSize;
            ZiSize.setPointSize(15);
            tmp->setFont(ZiSize);

            ui->listWidget->addItem(tmp);
        }
    }
}

void ChooseFile::on_pushButton_2_clicked()
{
    root =root+"../";

    qDebug()<<root;
    QDir dir;
    dir.setPath(root);
    slotShow(dir);
}
void ChooseFile::showEvent(QShowEvent *e)
{
    root="/sdcard/";
    QDir rootDir("/sdcard/");

    QStringList string;
    string << "*.jpg"<<"*.png";
    QFileInfoList list = rootDir.entryInfoList(string);
    QFileInfoList list1 = rootDir.entryInfoList(QDir::AllDirs| QDir::NoDotAndDotDot);
    showFileInfoList(list+list1);
}

void ChooseFile::on_BackBt_clicked()
{
    this->close();
}
