#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H


#include<QFileInfoList>
#include<QDir>
#include<QListWidgetItem>
#include"tcpclass.h"
namespace Ui {
class ChooseFile;
}

class ChooseFile : public QWidget,public TcpClass
{
    Q_OBJECT
signals:
    void SendFileName(QString);
public:
    explicit ChooseFile(QWidget *parent = 0);
    ~ChooseFile();
public:

    QString root;                       //让外界访问记录根目录
private:
    Ui::ChooseFile *ui;
    void showFileInfoList(QFileInfoList list);
    void slotShow(QDir dir);
    void showEvent(QShowEvent *e);
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
private slots:

    void slotShowDir(QListWidgetItem *item);
    void on_pushButton_2_clicked();
    void on_BackBt_clicked();

private:
    /****************************************88滑动区***************************************/
    QScrollBar *m_scrollBarV;                           //绑定滑动事件
    int press_y;
    int move_y;
    int release_y;
    QDateTime pressDateTime;
    QPropertyAnimation *animation;
};

#endif // CHOOSEFILE_H
