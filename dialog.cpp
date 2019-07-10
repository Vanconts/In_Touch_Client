 #include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    cur_txt = ui->Conv;
}
void Dialog::set_db(QSqlDatabase * db){
    this->db = db;
}
void Dialog::set_id(int num){
    this->user_id = num;
}
Dialog::~Dialog()
{
    std::vector<QTextEdit *>::iterator it;
    for(it=for_delete.begin();it!=for_delete.end();it++){
        delete *it;
    }
    delete ui;
}
void Dialog::set_curr_data(QString str,QString nick,int id,int flag1,int flag2 = 2){
    if(flag2 == 1){ //when data is coming from server
        QSqlQuery query;
        query.prepare("INSERT INTO messages (user_name_from, user_id_from, user_id_to, message_text)"
                      "VALUES (:nick,:user_id,:id,:text)");
        query.bindValue(":nick", nick);
        query.bindValue(":user_id", QString::number(this->user_id));
        query.bindValue(":id", QString::number(id));
        query.bindValue(":text", str);
        query.exec();
        if(map_id.contains(id)){
            text_id.value(id)->append("YOU:" + str);
        }
        else {
            add_d(nick,id);
            text_id.value(id)->append("YOU:" + str);
        }
        return;
    }
    if(flag2 == 0){ //when data is coming from local database
            if(map_id.contains(id)){
                text_id.value(id)->append("YOU:" + str);
            }
            else {
                add_d(nick,id);
                text_id.value(id)->append("YOU:" + str);
            }
            return;
    }
    if(flag1 == 1){
    QSqlQuery query;
    query.prepare("INSERT INTO messages (user_name_from, user_id_from,user_id_to, message_text)"
                  "VALUES (:nick,:id,:user_id,:text)");
    query.bindValue(":nick", nick);
    query.bindValue(":id", QString::number(id));
    query.bindValue(":user_id", QString::number(this->user_id));
    query.bindValue(":text", str);
    query.exec();
    if(map_id.contains(id)){
        text_id.value(id)->append(map_id.value(id)->text() + ":" + str);
    }
    else {
        add_d(nick,id);
        text_id.value(id)->append(nick + ":" + str);
    }
    return;
   }
    if(flag1 == 0){

        if(map_id.contains(id)){
            text_id.value(id)->append(map_id.value(id)->text() + ":" + str);
        }
        else {
            add_d(nick,id);
            text_id.value(id)->append(nick + ":" + str);
        }
    }
}
void Dialog::set_socket(SOCKET * s){
    this->s = s;
    d.setsock(s);
}
void Dialog::set_m(mthread * t){
    this->t = t;
    d.set_m(t);
};

void Dialog::on_send_clicked()
{
   int size;
   set_curr_data(ui->message->text(),map_id.value((id_map.value(cur_txt)))->text(),id_map.value(cur_txt),0,1);
   QString str =  ui->message->text();
   str = "um" + QString::number(id_map.value(cur_txt)) + "/" + str;
   size = str.length();
   char * arr = new char[size];
   strcpy(arr, str.toStdString().c_str());
   send(*s,&arr[0],strlen(arr),0);
   this->ui->message->clear();
   delete[] arr;
}

void Dialog::on_newdialog_clicked()
{
    d.exec();
}
void Dialog::add_d(QString str,int id){
    this->ui->listWidget->addItem(str);
    new_txt = new QTextEdit;
    for_delete.push_back(new_txt);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(new_txt->sizePolicy().hasHeightForWidth());
    new_txt->setSizePolicy(sizePolicy);
    d_map.insert(ui->listWidget->item(ui->listWidget->count()-1),new_txt);
    id_map.insert(new_txt,id);
    text_id.insert(id,new_txt);
    map_id.insert(id,ui->listWidget->item(ui->listWidget->count()-1));
}

void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    cur_txt = d_map.value(item);
    if(cur_txt != ui->Conv)
    {
        ui->centralLayout->removeWidget(ui->Conv);
        ui->centralLayout->addWidget(cur_txt, 1,0,1,2);
        ui->Conv->hide();
        ui->Conv = cur_txt;
        cur_txt->show();
    }
}

void Dialog::on_exitbutton_clicked()
{
    QSqlQuery query("delete from auto_data");
    query.exec();
    QSqlQuery query1("delete from messages");
    query1.exec();
    this->done(1);
}
void Dialog::news(){
    QSqlQuery query("select max(Timestamp) from messages");
    query.exec();
    if(!query.next() || query.value(0).toString() == ""){
        char to_send[3] = {'s','m','0'};
        send(*s,&to_send[0],sizeof(to_send),0);
    }else {
        QString str = "sm" + query.value(0).toString();
        QByteArray arr = str.toLocal8Bit();
        send(*s,arr.data(),arr.size(),0);
    }
}
void Dialog::sync_mess_db(QString nick, int id, QString date, QString mess_txt,int flag){
    if(flag == 1){
    QSqlQuery query;
    query.prepare("INSERT INTO messages (user_name_from, user_id_from,user_id_to, message_text ,Timestamp)"
                  "VALUES (:nick,:id,:user_id,:text,:date)");
    query.bindValue(":nick", nick);
    query.bindValue(":id", QString::number(id));
    query.bindValue(":user_id", QString::number(this->user_id));
    query.bindValue(":text", mess_txt);
    query.bindValue(":date", date);
    query.exec();
    set_curr_data(mess_txt,nick,id,0);
    }else if(flag == 0) {
        set_curr_data(mess_txt,nick,id,0,1);
    }
}
void Dialog::sync_all(){
    QSqlQuery query("select * from messages");
    query.exec();
    while(query.next()){
        QString nick = query.value(0).toString();
        int id = query.value(1).toInt();
        QString mess_txt = query.value(3).toString();
        if(this->user_id == id){
        id = query.value(2).toInt();
        set_curr_data(mess_txt,nick,id,0,0);
        }else{
            set_curr_data(mess_txt,nick,id,0);
        }
    }
}
