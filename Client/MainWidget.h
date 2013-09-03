#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "const.h"
#include "../public/ODownloader.h"

namespace Ui
{
    class MainWidget;
}

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent=0);
    ~MainWidget();
    void reSetUi();//重置UI，用于刷新配置文件之后
    void logoChange();
public slots:
    void onUList(QStringList &users);
    void onMsg(QString uname,QString msg);
    //下面这些是供ChatWidget绑定的
    void on_ActAbout_triggered();
    void on_ActMember_triggered();
    void on_ActVer_triggered();
    void on_ActJyboxIndex_triggered();
    void on_ActCheckNew_triggered();
    void on_ActDevBBS_triggered();
private:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤器，用于处理回车发送
    QSystemTrayIcon *trayIcon;//托盘
    QMenu *trayMenu;//托盘图标菜单
    Ui::MainWidget *ui;
    QLabel *LEnterToSend;//提示回车发送的QLabel
private slots:
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_UListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_DoSend_clicked();
    void on_ActChat_triggered();
    void on_ActUserInfo_triggered();
    void on_ActDebugOut_triggered();
    void on_ActQuit_triggered();
    void on_ActTray_triggered();
    void on_ActRefresh_triggered();
    void on_ActOptionFile_triggered();
    void on_ActSource_triggered();
    void on_ActProtocol_triggered();
    void on_ActChatLog_triggered();
};

#endif // MAINWIDGET_H
