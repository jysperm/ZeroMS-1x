#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "const.h"
#include "inline.h"

ChatWidget::ChatWidget(QWidget *parent):QMainWindow(parent),ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
}

ChatWidget::~ChatWidget()
{
    sDelete(ui);
}
