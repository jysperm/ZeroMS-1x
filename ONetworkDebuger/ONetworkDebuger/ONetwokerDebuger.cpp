#include "ONetwokerDebuger.h"
#include "ui_ONetwokerDebuger.h"

ONetwokerDebuger::ONetwokerDebuger(QWidget *parent):QWidget(parent),ui(new Ui::ONetwokerDebuger)
{
    ui->setupUi(this);
}

ONetwokerDebuger::~ONetwokerDebuger()
{
    delete ui;
}
