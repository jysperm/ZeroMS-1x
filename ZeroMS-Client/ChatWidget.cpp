#include "ChatWidget.h"
#include "ui_ChatWidget.h"

ChatWidget::ChatWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}
