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
    void closeEvent(QCloseEvent *event);
public slots:
    void logoChange();
    void onUList(QStringList &users);
    void onMsg(QString uname,QString msg);
private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    Ui::MainWidget *ui;
private slots:
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_ActABoutQt_triggered();
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
};

#endif // MAINWIDGET_H
