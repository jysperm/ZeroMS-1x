#include <QDesktopWidget>
#include <QDesktopServices>
#include <QKeyEvent>
#include "oclientcoreex.h"
#include "mainwidget.h"
#include "ui_chatwidget.h"

extern OClientCoreEx *cc;

//public:
ChatWidget::ChatWidget(QString uname,QWidget *parent):QMainWindow(parent),peerName(uname),ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    ui->LPeerName->setText(uname);

    //菜单栏信号槽
    connect(ui->ActMinSize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(ui->ActClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->ActSend, SIGNAL(triggered()), ui->DoSend, SLOT(click()));
    connect(ui->ActClear, SIGNAL(triggered()), ui->MsgArea, SLOT(clear()));
    connect(ui->ActAbout,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActAbout_triggered()));
    connect(ui->ActMember,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActMember_triggered()));
    connect(ui->ActABoutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->ActVer,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActVer_triggered()));
    connect(ui->ActJyboxIndex,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActJyboxIndex_triggered()));
    connect(ui->ActCheckNew,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActCheckNew_triggered()));
    connect(ui->ActUserBBS,SIGNAL(triggered()),cc->mainwidget,SLOT(on_ActDevBBS_triggered()));

    //为消息输入框安装用于回车发送的事件过滤器
    ui->MsgEdit->installEventFilter(this);

    reSetUi();
}

ChatWidget::~ChatWidget()
{
    emit onDelete(peerName);
    DELETE(ui);
}

void ChatWidget::reSetUi()
{
    setWindowTitle(tr("与%1聊天-%2").arg(peerName).arg(CLIENT_TITLE_NAME));
}

void ChatWidget::onMsg(QString msg)
{
    ui->MsgArea->append(tr("%1 : %2").arg(ui->LPeerName->text()).arg(Qt::escape(msg)));
}

//private:
void ChatWidget::closeEvent(QCloseEvent *event)
{
    //不知为何，如果不在这里显式调用析构函数，窗口就不会被关闭
    this->~ChatWidget();
}

bool ChatWidget::eventFilter(QObject *watched, QEvent *event)
{
    //注释请见mainwidget.cpp中的同名函数
    if(watched==ui->MsgEdit && event->type()==QEvent::KeyPress)
    {
        int key=(static_cast<QKeyEvent*>(event))->key();
        if(key==Qt::Key_Return || key==Qt::Key_Enter)
        {
            if(!((static_cast<QKeyEvent*>(event))->modifiers() & Qt::ControlModifier))
            {
                if((cc->config)["ENTER_TO_SEND"].toInt())
                {
                    on_DoSend_clicked();
                    return 1;
                }
            }
            else
            {
                if((cc->config)["CTRL_ENTER_TO_SEND"].toInt())
                {
                    on_DoSend_clicked();
                    return 1;
                }
                else
                {
                    ui->MsgEdit->insertPlainText(tr("\n"));
                }
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

//private slots:
void ChatWidget::on_DoSend_clicked()
{
    if(ui->MsgEdit->toPlainText().isEmpty())
    {
        ui->MsgArea->append(tr(">>提示:请不要发送空消息"));
        return;
    }
    cc->msgCMsg(peerName,ui->MsgEdit->toPlainText());
    ui->MsgArea->append(tr("%1 : %2").arg(cc->myname).arg(Qt::escape(ui->MsgEdit->toPlainText())));
    ui->MsgEdit->clear();
}

void ChatWidget::on_ActPeerInfo_triggered()
{
    QDesktopServices::openUrl(QUrl((cc->config)["USERINFO_URL"].toString().arg(peerName)));
}
