#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "const.h"
class QCloseEvent;
class QMenu;
class QWidget;
class ClientCoreEx;

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
};

#endif // MAINWIDGET_H
