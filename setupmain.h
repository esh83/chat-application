#ifndef SETUPMAIN_H
#define SETUPMAIN_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class SetupMain;
}

class SetupMain : public QDialog
{
    Q_OBJECT

public:
    explicit SetupMain(QWidget *parent = nullptr);
    ~SetupMain();



private slots:
    void on_btn_start_signup_clicked();

    void on_btn_start_login_clicked();

    void loginCheck(QString username,QString password);

    void on_btn_signup_cancel_clicked();

    void on_btn_login_cancel_clicked();

    void on_btn_login_login_clicked();

    void on_btn_signup_signup_clicked();

private:
    Ui::SetupMain *ui;
    bool check;
};

#endif // SETUPMAIN_H
