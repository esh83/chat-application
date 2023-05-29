#ifndef SETUPMAIN_H
#define SETUPMAIN_H

#include <QWidget>

namespace Ui {
class SetupMain;
}

class SetupMain : public QWidget
{
    Q_OBJECT

public:
    explicit SetupMain(QWidget *parent = nullptr);
    ~SetupMain();



private slots:
    void on_btn_start_signup_clicked();

    void on_btn_start_login_clicked();



    void on_btn_signup_cancel_clicked();

    void on_btn_login_cancel_clicked();

private:
    Ui::SetupMain *ui;
};

#endif // SETUPMAIN_H
