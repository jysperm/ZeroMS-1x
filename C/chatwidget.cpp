#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "const.h"
#include "inline.h"
#include "oclientcoreex.h"

extern OClientCoreEx *cc;

ChatWidget::ChatWidget(QString uname,QWidget *parent):QMainWindow(parent),ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    ui->LPeerName->setText(uname);
}

ChatWidget::~ChatWidget()
{
    cc->widgets.remove(ui->LPeerName->text());
    DELETE(ui);
}

void ChatWidget::closeEvent(QCloseEvent *event)
{
    delete this;
}
