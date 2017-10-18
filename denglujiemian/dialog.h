#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
signals:
    void ExitProduct();
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
   // void showEvent(QShowEvent *e);

    Ui::Dialog *ui;
private slots:
    void Btclick();
};

#endif // DIALOG_H
