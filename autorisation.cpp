#include "autorisation.h"
#include "ui_autorisation.h"
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <QMessageBox>
#include <QDebug>
using namespace std;
Autorisation::Autorisation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Autorisation)
{
    ui->setupUi(this);
}

Autorisation::~Autorisation()
{
    delete ui;
}
void CheckDatar(QString login,QString password){
        SOCKET sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
                WSACleanup();
        }
        else {
        SOCKADDR_IN anAddr;
        anAddr.sin_family = AF_INET;
        anAddr.sin_port = htons(80);
        anAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        connect(sock, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));
        QString tocheck ="r" + password + " " + login;
        QByteArray qarr = tocheck.toLocal8Bit();
        send(sock, qarr.data(), qarr.size(), 0);
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        recv(sock, buf, sizeof(buf), 0);
        QMessageBox ms;
        closesocket(sock);
        if(buf[0] == 't'){
            ms.setText("Registered Succesfully!");
        }
        else if(buf[0] == 'u' && buf[1] == 'e'){
            ms.setText("User already exists!");
        }
        else{
            ms.setText("Invalid Character found!");
        }
        ms.exec();
        }
}
void Autorisation::on_register_2_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
    CheckDatar(login,password);
    this->close();
}
