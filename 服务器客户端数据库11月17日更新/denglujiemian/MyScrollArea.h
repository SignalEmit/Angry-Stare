#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QObject>
#include <QWidget>
#include<QScrollArea>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QList>
#include<QScrollBar>
struct Item
{
    QLabel *HeadPicture;
    QLabel *Text;
    QHBoxLayout *HBoxLayout;
    Item(QLabel *HeadPicture_,QLabel *Text_,QHBoxLayout *HBoxLayout_):HeadPicture(HeadPicture_),Text(Text_),HBoxLayout(HBoxLayout_){}
};

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    void SetTextAndPicture(bool is_me,const QString &PicturePath,const QString &Text);
    void InitializtionScrollArea();                 //初始化界面
    ScrollArea(QWidget *parent=NULL);
    ~ScrollArea();
private:
    int trueListItemNumber=0;               //真正的聊天记录条数
    QHBoxLayout *HBoxLayout;
    QLabel *HeadPicture;
    QLabel *Text;
    QList<Item> ItemList;
    QWidget *base;
    int sWidth;
    int sHeight;
    QVBoxLayout *BaseVerticalLayout;
    int heigth_Change(float Percentage);
    int width_Change(float Percentage);
    void pushMyMessage(const QString &PicturePath,const QString &Text_);
    void pushFriendMessage(const QString &PicturePath,const QString &Text);
};

#endif // MYSCROLLAREA_H
