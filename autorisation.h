#ifndef AUTORISATION_H
#define AUTORISATION_H

#include <QDialog>
#include <winsock2.h>
namespace Ui {
class Autorisation;
}

class Autorisation : public QDialog
{
    Q_OBJECT

public:
    explicit Autorisation(QWidget *parent = nullptr);
    ~Autorisation();

private slots:
    void on_register_2_clicked();

private:
    Ui::Autorisation *ui;
};

#endif // AUTORISATION_H
