#include <QApplication>
#include <QDesktopWidget>
#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "const.h"
#include "inline.h"
#include "oclientcoreex.h"

extern OClientCoreEx *cc;

ChatWidget::ChatWidget(QString uname,QWidget *parent):QMainWindow(parent),ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    ui->LPeerName->setText(uname);

    setWindowTitle(tr("与%1聊天-%2").arg(ui->LPeerName->text()).arg(CLIENT_TITLE_NAME));
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

void ChatWidget::on_DoSend_clicked()
{
    cc->msgCMsg(ui->LPeerName->text(),ui->MsgEdit->toPlainText());
    ui->MsgArea->append(tr("%1 : %2").arg(cc->myname).arg(ui->MsgEdit->toPlainText()));
    ui->MsgEdit->setPlainText(tr(""));
}
