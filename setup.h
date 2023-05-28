#ifndef SETUP_H
#define SETUP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class setup; }
QT_END_NAMESPACE

class setup : public QWidget
{
    Q_OBJECT

public:
    setup(QWidget *parent = nullptr);
    ~setup();

private slots:
    void on_btn_login_clicked();

    void on_btn_signup_clicked();

private:
    Ui::setup *ui;
};
#endif // SETUP_H
