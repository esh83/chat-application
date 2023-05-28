#include "setup.h"
#include "ui_setup.h"

setup::setup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setup)
{
    ui->setupUi(this);
}

setup::~setup()
{
    delete ui;
}

