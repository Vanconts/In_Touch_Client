#include "mthread.h"
#include <QtCore>
#include <QMessageBox>
#include <dialog.h>
mthread::mthread()
{

}
mthread::~mthread()
{
}
void mthread::set_socket(SOCKET * s){
    this->s = s;
}
void mthread::run(){

    while(true)
    {
        memset(buf,0,sizeof(buf));
        recv(*s, buf, 4096, 0);
        if(buf[0]== 's'){
            if(buf[1] =='m'){
                QString str = buf;
                str.remove(0,2);
                if(str[0] != " "){
                QString nick = str.mid(0,str.indexOf(' '));
                str =  str.remove(0,str.indexOf(' ')+1);
                int id = str.mid(0,str.indexOf('/')).toInt();
                str =  str.remove(0,str.indexOf('/')+1);
                QString date = str.mid(0,str.indexOf('/'));
                str =  str.remove(0,str.indexOf('/')+1);
                emit sync(nick,id,date,str,1);
                }
                else {
                    str.remove(0,1);
                    QString nick = str.mid(0,str.indexOf(' '));
                    str =  str.remove(0,str.indexOf(' ')+1);
                    int id = str.mid(0,str.indexOf('/')).toInt();
                    str =  str.remove(0,str.indexOf('/')+1);
                    QString date = str.mid(0,str.indexOf('/'));
                    str =  str.remove(0,str.indexOf('/')+1);
                    emit sync(nick,id,date,str,0);
                }
            }else {
                for(int i = 0;i < strlen(buf);i++){
                    bufferr[i] = buf[i];
                }
                emit data_recv();
            }
            memset(buf,0,sizeof(buf));
        }
        else if(buf[0]== 'u' )
        {
            if( buf[1] =='m' )
                {
                    int id;
                    QString str = buf;
                    QString nick;
                    str.remove(0,2);
                    id = (str.mid(0,str.indexOf('/'))).toInt();
                    str.remove(0,str.indexOf('/') + 1);
                    nick = str.mid(0,str.indexOf('/'));
                    str =  str.remove(0,str.indexOf('/')+1);
                    emit execc(str,nick,id,1,2);
                    memset(buf,0,sizeof(buf));
                }
        }
    }
}
void mthread::ex(QString str){
    if(bufferr[1] == 't'){
        std::string str1 = bufferr;
        str1.erase(0,2);
        emit exe(str,stoi(str1));
        memset(bufferr,0,sizeof(bufferr));
    }
    else if(bufferr[1] == 'f'){
        memset(bufferr,0,sizeof(bufferr));
    }
}

