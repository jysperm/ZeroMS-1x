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
#include "inline.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "oclientcoreex.h"
#include "chatwidget.h"
#include "ui_chatwidget.h"

extern OClientCoreEx *cc;

//public:
MainWidget::MainWidget(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    ui->LMyName->setText(cc->myname);
    setWindowTitle(QString("%1  %2").arg(cc->myname).arg(CLIENT_TITLE_NAME));

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
}

MainWidget::~MainWidget()
{
    DELETE(trayIcon);
    DELETE(trayMenu);
    DELETE(ui);
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
    QMessageBox::information(0,tr("合作开发人员名单"),file.readAll());
}

void MainWidget::on_ActRefresh_triggered()
{
    cc->msgAskUList();
}

void MainWidget::on_UListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(cc->widgets.contains(item->text()))
    {
        cc->widgets[item->text()]->activateWindow();
    }
    else
    {
        ChatWidget *cp=new ChatWidget(item->text());
        cc->widgets.insert(item->text(),cp);
        cp->show();
    }
}
