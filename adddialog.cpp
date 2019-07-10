#include "adddialog.h"
#include "ui_adddialog.h"
#include <QDebug>
#include <dialog.h>
#include <QEventLoop>
AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;

}
void AddDialog::setsock(SOCKET *s){
    this->s = s;
}
void AddDialog::set_m(mthread * t){
    this->t = t;
    QObject::connect(this,SIGNAL(ex(QString)),
                     t,SLOT(ex(QString)));
};
void AddDialog::on_pushButton_clicked()
{
    int size;
    QString str = "sc" + ui->lineEdit->text();
    size = str.length();
    char * arr = new char[size];
    strcpy(arr, str.toStdString().c_str());
    QEventLoop eventLoop;
    QObject::connect(t, SIGNAL(data_recv()), &eventLoop, SLOT(quit()));
    send(*s,arr,strlen(arr),0);
    eventLoop.exec();
    emit ex(ui->lineEdit->text());
    delete[] arr;
    this->done(1);
}


