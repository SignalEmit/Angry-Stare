#include "widget.h"
#include <QApplication>
#include<QFile>
#include<QSqlDatabase>
#include<QSqlQuery>
#include"interface.h"
//QSqlDatabase db;
QString UserAccount;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*********************************数据库*************************/
     /*
   if(QSqlDatabase::contains("qt_sql_default_connection"))
     db = QSqlDatabase::database("qt_sql_default_connection");
   else
     db = QSqlDatabase::addDatabase("QSQLITE");

   QFile file("MyDateBase.db");
   if(file.exists())
   {

       db.setDatabaseName("MyDateBase.db");
   }
   else
   {
       db.setDatabaseName("MyDateBase.db");
       db.open();
       QSqlQuery BuildNewTable;
       BuildNewTable.exec("CREATE TABLE [LoginUserTable]("
                          "[UserAccount] VARCHAR(50) NOT NULL,"
                          "[Password] VARCHAR(30), "
                          "[is_Login] BOOL(2),"
                          "UNIQUE([UserAccount]));");
       BuildNewTable.exec("CREATE TABLE [SearchHistory]("
                          "[History] VARCHAR(50),"
                          "UNIQUE([History]));");

      db.close();
   }
   db.open();
   QSqlQuery check("select UserAccount from LoginUserTable where is_Login='true'");
   check.exec();
   if(check.next())
   {
       UserAccount=check.value("UserAccount").toString();
       Interface s;
       s.show();
       return a.exec();
   }
   else
   {
       Widget w;
       w.show();
       return a.exec();
   }
    db.close();
    */
    Widget w;
    return a.exec();
}
