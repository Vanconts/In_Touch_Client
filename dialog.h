#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <winsock2.h>
#include "mthread.h"
#include <QObject>
#include <adddialog.h>
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <vector>
#include <QtDebug>
#include <QMessageBox>
#include <QTextEdit>
#include <QLayoutItem>
#include <string>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    void set_id(int);
    void set_socket(SOCKET * s);
    void set_m(mthread *);
    void set_db(QSqlDatabase * db);
    ~Dialog();
public slots:
    void set_curr_data(QString,QString,int,int,int );
    void add_d(QString,int);
private slots:
    void sync_all();
    void news();
    void on_send_clicked();
    void on_newdialog_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_exitbutton_clicked();
    void sync_mess_db(QString,int,QString,QString,int);
private:
    int user_id;
    bool * f;
    QSqlDatabase * db;
    Ui::Dialog *ui;
    AddDialog d;
    char cur_data[4096];
    mthread * t;
    SOCKET * s;
    QTextEdit * cur_txt;
    QTextEdit * new_txt;
    QMap <QListWidgetItem *,QTextEdit *> d_map;
    QMap <QTextEdit *,int> id_map;
    QMap <int,QListWidgetItem *> map_id;
    QMap <int,QTextEdit *> text_id;
    std::vector <QTextEdit *> for_delete;
};

#endif // DIALOG_H
