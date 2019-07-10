#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <winsock2.h>
#include <windows.h>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
void MainWindow::check(){
    anAddr.sin_family = AF_INET;
    anAddr.sin_port = htons(80);
    anAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user_data.db");
    db.open();
    if(db.isOpen()){
        QSqlQuery query("select login,password,flag from auto_data");
        query.exec();
        if(!query.next() || query.value(2).toString() == "0" ){
            this->show();
        }
        else if(query.value(2).toString() == "1"){
          CheckDatal(query.value(0).toString(),query.value(1).toString(),0);
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
    WSACleanup();
}


void MainWindow::CheckDatal(QString login,QString password,bool k){
        if(con == false){
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
                WSACleanup();
                return;
        }
        int iResult = ::connect(sock, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));
            if (iResult == SOCKET_ERROR) {
                    iResult = closesocket(sock);
                    if (iResult == SOCKET_ERROR)
                    WSACleanup();
                    return;
                }
        con = true;
        }
        if(con){
        QString tocheck ="l" + password + " " + login;
        QByteArray qarr = tocheck.toLocal8Bit();
        send(sock, qarr.data(), qarr.size(), 0);
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        recv(sock, buf, sizeof(buf), 0);
        if(buf[0] == 't'){
            Dialog d;
            mthread t1;
            tocheck = buf;
            tocheck.remove(0,1);
            d.set_id(tocheck.toInt());
            if(k){
                QSqlQuery query;
                    query.prepare("INSERT INTO auto_data (login, password, flag) "
                                  "VALUES (:login, :password, :k)");
                    query.bindValue(":login", login);
                    query.bindValue(":password", password);
                    query.bindValue(":k", k);
                    query.exec();
            }
            d.set_m(&t1);
            d.set_socket(&sock);
            d.set_db(&db);
            t1.set_socket(&sock);
            QObject::connect(&t1,SIGNAL(execc(QString,QString,int,int,int)),
                                &d,SLOT(set_curr_data(QString,QString,int,int,int)),Qt::QueuedConnection);
            QObject::connect(&t1,SIGNAL(exe(QString,int)),
                                &d,SLOT(add_d(QString,int)),Qt::QueuedConnection);
            QObject::connect(&t1,SIGNAL(sync(QString,int,QString,QString,int)),
                                &d,SLOT(sync_mess_db(QString,int,QString,QString,int)),Qt::QueuedConnection);
            t1.start();
            if(this->isVisible()){
                this->hide();
            }
            QMetaObject::invokeMethod(&d,"news",Qt::QueuedConnection);
            QMetaObject::invokeMethod(&d,"sync_all",Qt::QueuedConnection);
            if(d.exec()){
                t1.terminate();
                closesocket(sock);
                con = false;
                this->show();
            }
            else{
                    t1.terminate();
                    closesocket(sock);
                    QCoreApplication::quit();
                }
        }
        else {
            closesocket(sock);
            con = false;
            QMessageBox errorms;
            errorms.setText("Wrong login or password,please try again");
            errorms.exec();
            if(!k){
                QSqlQuery query("delete from auto_data");
                query.exec();
                this->show();
            }
        }
    }
}

void MainWindow::on_Login_clicked()
{
    QString login = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    CheckDatal(login,password,1);
}

void MainWindow::on_Singn_in_clicked()
{
    Autorisation autor;
    autor.resize(500,350);
    autor.exec();
}
