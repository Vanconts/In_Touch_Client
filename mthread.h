#ifndef THREAD_H
#define THREAD_H
#include <QtCore>
#include <QObject>
#include <QtDebug>
#include <QThread>
#include <winsock2.h>
#include <string>
class Dialog;
class mthread : public QThread
{
    Q_OBJECT
signals:
    void data_recv();
    void sync(QString,int,QString,QString,int);
    void execc(QString ,QString,int,int,int );
    void exe(QString,int);
public:
    mthread();
    void run();
    void set_socket(SOCKET * s);
    ~mthread();
private:
    SOCKET * s;
    char buf[5100];
    char bufferr[5100];
public slots:
    void ex(QString);
};

#endif // THREAD_H
