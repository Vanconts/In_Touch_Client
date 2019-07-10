#include "mainwindow.h"
#include <QApplication>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <mthread.h>
using namespace std;


int main(int argc, char *argv[])
{
    WSADATA WsaData;
    int err = WSAStartup(0x0101, &WsaData);
    if (err == SOCKET_ERROR)
    {
        qDebug() << "WSAStartup() failed: %ld\n" << GetLastError();
    }
    QApplication a(argc, argv);
    MainWindow w;
    QMetaObject::invokeMethod(&w,"check",Qt::QueuedConnection);
    return a.exec();
}
