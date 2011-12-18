#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QImage>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QUrl>
#include <QWidget>
#include "const.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "oclientcoreex.h"
#include "chatwidget.h"
#include "ui_chatwidget.h"

extern OClientCoreEx *cc;

//public:
MainWidget::MainWidget(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWidget),LEnterToSend(0)
{
    ui->setupUi(this);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //托盘
    trayIcon=new QSystemTrayIcon(this);
    QIcon icon(":/images/40_0ms.png");
    trayMenu=new QMenu(this);
    trayMenu->addAction(ui->ActQuit);
    trayMenu->addSeparator();
    trayIcon->setIcon(icon);
    trayIcon->show();
    trayIcon->setToolTip(CLIENT_TITLE_NAME);
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));

    //菜单栏信号槽
    connect(ui->ActRefresh,SIGNAL(triggered()),cc,SLOT(msgAskUList()));
    connect(ui->ActMinSize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(ui->ActUserBBS, SIGNAL(triggered()), ui->ActDevBBS, SLOT(trigger()));
    connect(ui->ActSend, SIGNAL(triggered()), ui->DoSend, SLOT(click()));
    connect(ui->ActClear, SIGNAL(triggered()), ui->MsgArea, SLOT(clear()));

    //回车发送
    LEnterToSend=new QLabel(this);
    statusBar()->addWidget(LEnterToSend);
    //为消息输入框安装事件过滤器
    ui->MsgEdit->installEventFilter(this);

    reSetUi();
}

MainWidget::~MainWidget()
{
    DELETE(trayIcon);
    DELETE(trayMenu);
    DELETE(ui);
}

void MainWidget::reSetUi()
{
    ui->LMyName->setText(cc->myname);
    setWindowTitle(QString("%1  %2").arg(cc->myname).arg(CLIENT_TITLE_NAME));
    LEnterToSend->setText(tr("回车键发送，Ctrl+回车换行哦~"));
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

//public slots:
void MainWidget::logoChange()
{
    //设置我的头像
    QImage image((QDir::currentPath()+"/cahce/logo/%1.png").arg(cc->myname));
    image=image.scaled(60,60);
    ui->MyLogo->setPixmap(QPixmap::fromImage(image));
    ui->MyLogo->adjustSize();
}

void MainWidget::onUList(QStringList &users)
{
    ui->UListWidget->clear();
    foreach(QString i,users)
    {
        ui->UListWidget->addItem(i);
    }
}

void MainWidget::onMsg(QString uname,QString msg)
{
    ui->MsgArea->append(tr("%1 : %2").arg(uname).arg(Qt::escape(msg)));
}

bool MainWidget::eventFilter(QObject *watched, QEvent *event)
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

//private slots:
void MainWidget::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            show();
            activateWindow();
            break;
        default:
            break;
    }
}

void MainWidget::on_ActABoutQt_triggered()
{
    QMessageBox::aboutQt(0,tr("感谢Qt"));
}

void MainWidget::on_ActQuit_triggered()
{
    //加这句是为了调用各个聊天窗体的析构函数
    qApp->closeAllWindows();
    this->~MainWidget();
    qApp->quit();
}

void MainWidget::on_ActTray_triggered()
{
    trayIcon->showMessage(tr("已最小化到托盘"), CLIENT_TITLE_NAME, QSystemTrayIcon::Information, 5000);
    close();
}

void MainWidget::on_ActSource_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/code"));
}

void MainWidget::on_ActProtocol_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/wiki/0-1/connect-protocol"));
}

void MainWidget::on_ActDevBBS_triggered()
{
    QDesktopServices::openUrl(QUrl("http://jybox.net/bbs/thread.php?fid-19.html"));
}

void MainWidget::on_ActAbout_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/about"));
}

void MainWidget::on_ActJyboxIndex_triggered()
{
    QDesktopServices::openUrl(QUrl("http://jybox.net"));
}

void MainWidget::on_ActCheckNew_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/down"));
}

void MainWidget::on_ActVer_triggered()
{
    QMessageBox::information(0,CLIENT_TITLE_NAME,CLIENT_NAME+"\n"+CLIENT_VER_NAME);
}

void MainWidget::on_ActMember_triggered()
{
    QFile file(":/Text/members.html");
    file.open(QFile::ReadOnly);
    QMessageBox msgBox;
    msgBox.resize(1000,500);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("合作开发人员名单"));
    msgBox.setText(file.readAll());
    msgBox.exec();
}

void MainWidget::on_ActRefresh_triggered()
{
    cc->msgAskUList();
}

void MainWidget::on_UListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    cc->showChatWidget(item->text());
}

void MainWidget::on_DoSend_clicked()
{
    if(ui->MsgEdit->toPlainText().isEmpty())
    {
        ui->MsgArea->append(tr(">>提示:请不要发送空消息"));
        return;
    }
    cc->msgCMsg(MAIN_GROUP,ui->MsgEdit->toPlainText());
    ui->MsgEdit->clear();
}

void MainWidget::on_ActChat_triggered()
{
    QList<QListWidgetItem*> users=ui->UListWidget->selectedItems();
    for(QList<QListWidgetItem*>::Iterator i=users.begin();i!=users.end();++i)
    {
        cc->showChatWidget((*i)->text());
    }
}

void MainWidget::on_ActUserInfo_triggered()
{
    QList<QListWidgetItem*> users=ui->UListWidget->selectedItems();
    for(QList<QListWidgetItem*>::Iterator i=users.begin();i!=users.end();++i)
    {
        QDesktopServices::openUrl(QUrl((cc->config)["USERINFO_URL"].toString().arg((*i)->text())));
    }
}

void MainWidget::on_ActDebugOut_triggered()
{
    QMessageBox::information(0,tr("调试输出"),tr("该版本不提供调试输出功能"));
}

void MainWidget::on_ActOptionFile_triggered()
{
    QDesktopServices::openUrl(QUrl(PUBLIC_CONFIG_FILE));
}
