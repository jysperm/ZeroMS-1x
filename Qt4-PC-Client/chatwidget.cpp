#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "const.h"
#include "oclientcoreex.h"

extern OClientCoreEx *cc;

ChatWidget::ChatWidget(QString uname,QWidget *parent):QMainWindow(parent),ui(new Ui::ChatWidget),peerName(uname)
{
    ui->setupUi(this);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    ui->LPeerName->setText(uname);

    //为消息输入框安装用于回车发送的事件过滤器
    ui->MsgEdit->installEventFilter(this);

    reSetUi();
}

void ChatWidget::reSetUi()
{
    setWindowTitle(tr("与%1聊天-%2").arg(peerName).arg(CLIENT_TITLE_NAME));
}

bool ChatWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->MsgEdit && event->type()==QEvent::KeyPress)
    {
        //如果是ui->MsgEdit发出的消息，且是按键消息
        int key=(static_cast<QKeyEvent*>(event))->key();
        if(key==Qt::Key_Return || key==Qt::Key_Enter)
        {
            //如果按键是回车
            if(!((static_cast<QKeyEvent*>(event))->modifiers() & Qt::ControlModifier))
            {
                //如果Ctrl没有被按下，就调用发送函数
                on_DoSend_clicked();
                return 1;
            }
            else
            {
                //如果Ctrl被按下，在当前位置插入换行
                ui->MsgEdit->insertPlainText(tr("\n"));
            }
        }
    }
    //将未处理的事件交给基类处理
    return QMainWindow::eventFilter(watched, event);
}

ChatWidget::~ChatWidget()
{
    cc->widgets.remove(ui->LPeerName->text());
    DELETE(ui);
}

void ChatWidget::closeEvent(QCloseEvent *event)
{
    this->~ChatWidget();
}

void ChatWidget::onMsg(QString msg)
{
    ui->MsgArea->append(tr("%1 : %2").arg(ui->LPeerName->text()).arg(Qt::escape(msg)));
}

void ChatWidget::on_DoSend_clicked()
{
    cc->msgCMsg(ui->LPeerName->text(),ui->MsgEdit->toPlainText());
    ui->MsgArea->append(tr("%1 : %2").arg(cc->myname).arg(Qt::escape(ui->MsgEdit->toPlainText())));
    ui->MsgEdit->clear();
}
