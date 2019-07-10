#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <winsock2.h>
#include <windows.h>
#include <QMessageBox>
#include <mthread.h>
#include <QSqlDatabase>
#include <QSqlQuery>
class Dialog;
namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT
signals:
    void ex(QString);
public:
    explicit AddDialog(QWidget *parent = nullptr);
    void add_new();
    void set_m(mthread *);
    void setsock(SOCKET *);
    ~AddDialog();
private slots:
    void on_pushButton_clicked();
private:
    Ui::AddDialog *ui;
    mthread * t;
    SOCKET * s;
};

#endif // ADDDIALOG_H
