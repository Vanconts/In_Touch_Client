#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "autorisation.h"
#include <QMainWindow>
#include "dialog.h"
#include <mthread.h>
#include <adddialog.h>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
public slots :

private slots:
    void on_Login_clicked();
    void on_Singn_in_clicked();
    void check();
private:
    Ui::MainWindow *ui;
    SOCKET sock;
    void CheckDatal(QString,QString,bool);
    QSqlDatabase db;
    bool con = false;
    SOCKADDR_IN anAddr;
};

#endif // MAINWINDOW_H
