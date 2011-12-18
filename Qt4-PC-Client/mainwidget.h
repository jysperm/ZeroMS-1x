#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "const.h"
class ClientCoreEx;
class QCloseEvent;
class QListWidgetItem;
class QMenu;
class QWidget;
class QLabel;

namespace Ui
{
    class MainWidget;
}

class MainWidget:public QMainWindow
{
    Q_OBJECT
    friend class ClientCoreEx;
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void reSetUi();//重置UI，用于刷新配置文件之后
public slots:
    void logoChange();
    void onUList(QStringList &users);
    void onMsg(QString uname,QString msg);
private:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;//托盘图标菜单
    Ui::MainWidget *ui;
    QLabel *LEnterToSend;//提示回车发送的QLabel
private slots:
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_ActQuit_triggered();
    void on_ActTray_triggered();
    void on_ActSource_triggered();
    void on_ActProtocol_triggered();
    void on_ActDevBBS_triggered();
    void on_ActAbout_triggered();
    void on_ActJyboxIndex_triggered();
    void on_ActCheckNew_triggered();
    void on_ActVer_triggered();
    void on_ActMember_triggered();
    void on_ActRefresh_triggered();
    void on_UListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_DoSend_clicked();
    void on_ActChat_triggered();
    void on_ActUserInfo_triggered();
    void on_ActDebugOut_triggered();
    void on_ActOptionFile_triggered();
};

#endif // MAINWIDGET_H
